/***********************************************************
 **  ardrone_sendata_client.h
 **
 **  This is a modified version of ardrone_navdata_client.h 
 **  rewritten to accomidate data input from an outboard 
 **  sensor array.
 **
 **  (C) J.Weeks Tufts University 07/18/2012
 **
 **  ardrone_navdata_client.h (C) PARROT SA 2007-2011
 **
 ***********************************************************/

#ifndef _ARDRONE_SENDATA_CLIENT_H_
#define _ARDRONE_SENDATA_CLIENT_H_

#include <VP_Os/vp_os_types.h>
#include <VP_Api/vp_api_thread_helper.h>

//#include <ardrone_api.h>
#include <ardrone_tool/Sendata/sendata.h>
#include <ardrone_tool/Control/ardrone_sendata_control.h>
#include <ardrone_tool/Sendata/ardrone_general_sendata.h>
//#include <config.h>

#define SENDATA_MAX_RETRIES	5

// Facility to declare a set of sendata handler
// Handler to resume control thread is mandatory
#define BEGIN_SENDATA_HANDLER_TABLE \
  ardrone_sendata_handler_t ardrone_sendata_handler_table[] = {

#define END_SENDATA_HANDLER_TABLE					\
  { ardrone_general_sendata_init, ardrone_general_sendata_process, ardrone_general_sendata_release, NULL }, \
  { ardrone_sendata_control_init, ardrone_sendata_control_process, ardrone_sendata_control_release, NULL }, \
  { NULL, NULL, NULL, NULL }						\
};

#define SENDATA_HANDLER_TABLE_ENTRY( init, process, release, init_data_ptr ) \
  { (ardrone_sendata_handler_init_t)init, process, release, init_data_ptr },

typedef C_RESULT (*ardrone_sendata_handler_init_t)( void* data );
typedef C_RESULT (*ardrone_sendata_handler_process_t)( const sendata_unpacked_t* const sendata );
typedef C_RESULT (*ardrone_sendata_handler_release_t)( void );

typedef struct _ardrone_sendata_handler_t {
  ardrone_sendata_handler_init_t    init;
  ardrone_sendata_handler_process_t process;
  ardrone_sendata_handler_release_t release;

  void*                             data; // Data used during initialization
} ardrone_sendata_handler_t;

extern ardrone_sendata_handler_t ardrone_sendata_handler_table[] WEAK;

uint32_t ardrone_sendata_client_get_num_retries(void);
C_RESULT ardrone_sendata_client_init(void);
C_RESULT ardrone_sendata_client_suspend(void);
C_RESULT ardrone_sendata_client_resume(void);
C_RESULT ardrone_sendata_client_shutdown(void);
C_RESULT ardrone_sendata_open_server(void);

PROTO_THREAD_ROUTINE( sendata_update , nomParams );

#endif // _ARDRONE_SENDATA_H_
