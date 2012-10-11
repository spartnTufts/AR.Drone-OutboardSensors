/***********************************************************
 **  ardrone_sendata_client.c
 **
 **  This is a modified version of ardrone_navdata_client.c 
 **  rewritten to accomidate data input from an outboard 
 **  sensor array.
 **
 **  (C) J.Weeks Tufts University 07/18/2012
 **  
 **  ardrone_navdata_client.c (C) PARROT SA 2007-2011
 **
 ***********************************************************/

//#include <errno.h>
//#include <string.h>

//#include <config.h>

#include <VP_Os/vp_os_print.h>
//#include <VP_Com/vp_com.h>

//#include <ardrone_api.h>
#include <ardrone_tool/ardrone_tool.h>
#include <ardrone_tool/Sendata/ardrone_sendata_client.h>
#include <ardrone_tool/Com/config_com.h>

#ifndef _WIN32
#include <sys/socket.h>
//#include <sys/ioctl.h>
//#include <netinet/in.h>
//#include <netinet/tcp.h>
//#include <unistd.h>
#endif

static bool_t sendata_thread_in_pause = TRUE;
static bool_t bContinue = TRUE;
static uint32_t num_retries = 0; 
static vp_os_cond_t sendata_client_condition;
static vp_os_mutex_t sendata_client_mutex;

static vp_com_socket_t sendata_socket;
static Read sendata_read      = NULL;
static Write sendata_write    = NULL;

static uint8_t sendata_buffer[SENDATA_MAX_SIZE];
sendata_unpacked_t sendata_unpacked;

char wifi_arduino_ip[256]={WIFI_ARDUINO_IP};

C_RESULT ardrone_sendata_client_init(void)
{
  C_RESULT res;

  COM_CONFIG_SOCKET_NAVDATA(&sendata_socket, VP_COM_CLIENT, SENDATA_PORT, wifi_arduino_ip);
  sendata_socket.protocol = VP_COM_UDP;
  sendata_socket.is_multicast = 1;      // enable multicast for Sendata
  sendata_socket.multicast_base_addr = MULTICAST_BASE_ADDR;

  vp_os_mutex_init(&sendata_client_mutex);
  vp_os_cond_init(&sendata_client_condition, &sendata_client_mutex);
	
  res = C_OK;

  return res;
}

C_RESULT ardrone_sendata_client_suspend(void)
{
  vp_os_mutex_lock(&sendata_client_mutex);
  sendata_thread_in_pause = TRUE;
  vp_os_mutex_unlock(&sendata_client_mutex);	
  
  return C_OK;
}

C_RESULT ardrone_sendata_client_resume(void)
{
  vp_os_mutex_lock(&sendata_client_mutex);
  vp_os_cond_signal(&sendata_client_condition);
  sendata_thread_in_pause = FALSE;
  vp_os_mutex_unlock(&sendata_client_mutex);	

  return C_OK;
}

C_RESULT ardrone_sendata_open_server(void)
{
  // Flag value :
  // 1 -> Unicast
  // 2 -> Multicast
  int32_t flag = 1, len = sizeof(flag);

  if( sendata_write != NULL )
  {
    if (sendata_socket.is_multicast == 1)
      flag = 2;

    sendata_write(&sendata_socket, (const int8_t*) &flag, &len);
  }

  return C_OK;
}

DEFINE_THREAD_ROUTINE( sendata_update, nomParams )
{
  C_RESULT res;
  int32_t  i, size;
  uint32_t cks, sendata_cks, sequence = SENDATA_SEQUENCE_DEFAULT-1;
  struct timeval tv;
#ifdef _WIN32
  int timeout_for_windows=1000/*milliseconds*/;
#endif


  sendata_t* sendata = (sendata_t*) &sendata_buffer[0];

  tv.tv_sec   = 1/*second*/;
  tv.tv_usec  = 0;

  res = C_OK;

  if( VP_FAILED(vp_com_open(COM_NAVDATA(), &sendata_socket, &sendata_read, &sendata_write)) )
  {
    printf("VP_Com : Failed to open socket for sendata\n");
    res = C_FAIL;
  }

  if( VP_SUCCEEDED(res) )
  {
    PRINT("Thread sendata_update in progress...\n");

#ifdef _WIN32
    setsockopt((int32_t)sendata_socket.priv, SOL_SOCKET, SO_RCVTIMEO,	\
	       (const char*)&timeout_for_windows, sizeof(timeout_for_windows));
    if(sendata_write)
      {	int sizeinit = 5; sendata_write( (void*)&sendata_socket, (int8_t*)"Init", &sizeinit ); }
#else
    setsockopt((int32_t)sendata_socket.priv, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));
#endif

    i = 0;
    while( ardrone_sendata_handler_table[i].init != NULL )
      {
	// If init failed for an handler, set its process function to null
	// and keep its release function for cleanup
	if( VP_FAILED( ardrone_sendata_handler_table[i].init(ardrone_sendata_handler_table[i].data) ) )
	  ardrone_sendata_handler_table[i].process = NULL;

	i ++;
      }

    sendata_thread_in_pause = FALSE;
    while( VP_SUCCEEDED(res) 
           && !ardrone_tool_exit() 
           && bContinue )
      {
	if(sendata_thread_in_pause)
	  {
	    vp_os_mutex_lock(&sendata_client_mutex);
	    num_retries = SENDATA_MAX_RETRIES + 1;
	    vp_os_cond_wait(&sendata_client_condition);
	    vp_os_mutex_unlock(&sendata_client_mutex);
	  }

	if( sendata_read == NULL )
	  {
	    res = C_FAIL;
	    continue;
	  }

	size = SENDATA_MAX_SIZE;
	sendata->header = 0; // Soft reset
	res = sendata_read( (void*)&sendata_socket, (int8_t*)&sendata_buffer[0], &size );

#ifdef _WIN32	
	if( size <= 0 )
#else
	if( size == 0 )
#endif
	  {
	    // timeout
	    PRINT("Timeout when reading sendatas - resending a sendata request on port %i\n",SENDATA_PORT);
	    // Resend a request to the arduino to get sendatas 
	    ardrone_sendata_open_server();
	    sequence = SENDATA_SEQUENCE_DEFAULT-1;
	    num_retries++;
	  } 
	else
	  num_retries = 0;

	if( VP_SUCCEEDED( res ) )
	  {
	    if( sendata->header == SENDATA_HEADER )
	      {
		if( sendata->sequence > sequence )
		  {
		    i = 0;

		    ardrone_sendata_unpack_all(&sendata_unpacked, sendata, &sendata_cks);
		    cks = ardrone_sendata_compute_cks( &sendata_buffer[0], size - sizeof(sendata_cks_t) );
		    
		    if( cks == sendata_cks )
		      {
			while( ardrone_sendata_handler_table[i].init != NULL )
			  {
			    if( ardrone_sendata_handler_table[i].process != NULL )
			      ardrone_sendata_handler_table[i].process( &sendata_unpacked );

			    i++;
			  }
		      }
		    else
		      {
			PRINT("[Sendata] Checksum failed : %d (distant) / %d (local)\n", sendata_cks, cks);
		      }
		  }
		else
		  {
		    PRINT("[Sendata] Sequence pb : %d (distant) / %d (local)\n", sendata->sequence, sequence);
		  }
		sequence = sendata->sequence;
	      }
	  }
      }
    
    // Release resources allocated by handlers
    i = 0;
    while( ardrone_sendata_handler_table[i].init != NULL )
      {
	ardrone_sendata_handler_table[i].release();
	i ++;
      }
  }

  vp_com_close(COM_NAVDATA(), &sendata_socket);

  DEBUG_PRINT_SDK("Thread sendata_update ended\n");

  return (THREAD_RET)res;
}

uint32_t ardrone_sendata_client_get_num_retries(void)
{
  return num_retries;
}

C_RESULT ardrone_sendata_client_shutdown(void)
{
  bContinue = FALSE;

  return C_OK;
}
