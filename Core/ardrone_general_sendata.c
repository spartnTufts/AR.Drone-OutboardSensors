/***********************************************************
 **  ardrone_general_sendata.c
 **
 **  This is a modified version of ardrone_general_navdata.c 
 **  rewritten to accomidate data input from an outboard 
 **  sensor array.
 **
 **  (C) J.Weeks Tufts University 07/18/2012
 **
 **  ardrone_general_navdata.c (C) PARROT SA 2007-2011
 **
 ***********************************************************/

//#include <stdio.h>
//#include <sys/time.h>
//#include <ardrone_api.h>
//#include <ardrone_tool/ardrone_tool.h>
//#include <ardrone_tool/ardrone_tool_configuration.h>
#include <ardrone_tool/Sendata/ardrone_sendata_client.h>
#include <ardrone_tool/Sendata/ardrone_general_sendata.h>
//#include <utils/ardrone_gen_ids.h>


static SENDATA_REQUEST_STATE sendataState;
static int sendataNeeded = 0;

#define _GENERAL_SENDATA_DEBUG (0)
#define _GENERAL_SENDATA_DEBUG_PREFIX "General Sendata : "
#if _GENERAL_SENDATA_DEBUG
#define PRINTDBG(...)						     \
do								     \
{								     \
  printf ("[%d] %s", __LINE__, _GENERAL_SENDATA_DEBUG_PREFIX);	     \
  printf (__VA_ARGS__);						     \
} while (0)
#else
#define PRINTDBG(...)
#endif

void sendataCallback (int res)
{
  PRINTDBG ("Sendata callback called with result %d\n", res);
  if (0 != res)
    {
      PRINTDBG ("State : %d\n", sendataState);
      switch (sendataState)
	{
	case SENDATA_REQUEST_IN_PROGRESS: 
	  // Sendata request got acknowledged by the Arduino
	  sendataState = SENDATA_REQUEST_IDLE;
	  break;
	case SENDATA_REQUEST_IDLE:
	case SENDATA_REQUEST_NEEDED:
	default:
	  break;
	}
    }
  PRINTDBG ("End of sendata callback call\n");
}

C_RESULT ardrone_general_sendata_init( void* data )
{
  sendataState = SENDATA_REQUEST_IDLE;
  sendataNeeded = 1;
  
  return C_OK;
}

C_RESULT ardrone_general_sendata_process( const sendata_unpacked_t* const pnd )
{
  if (sendataState == SENDATA_REQUEST_IDLE)
    {
      sendataState = SENDATA_REQUEST_NEEDED; 
    }

  return C_OK;
}

C_RESULT ardrone_general_sendata_release( void )
{
  return C_OK;
}
