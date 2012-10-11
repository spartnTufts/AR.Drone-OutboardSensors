/***********************************************************
 **  ardrone_general_sendata.h
 **
 **  This is a modified version of ardrone_general_navdata.h 
 **  rewritten to accomidate data input from an outboard 
 **  sensor array.
 **
 **  (C) J.Weeks Tufts University 07/18/2012
 **
 **  ardrone_general_navdata.h (C) PARROT SA 2007-2011
 **
 ***********************************************************/

#ifndef _ARDRONE_SENDATA_GENERAL_H_
#define _ARDRONE_SENDATA_GENERAL_H_

#include <ardrone_tool/Sendata/ardrone_sendata_client.h>

typedef enum _SENDATA_REQUEST_STATE_
{
    SENDATA_REQUEST_IDLE,
    SENDATA_REQUEST_NEEDED,
    SENDATA_REQUEST_IN_PROGRESS,
} SENDATA_REQUEST_STATE;

C_RESULT ardrone_general_sendata_init( void* data );
C_RESULT ardrone_general_sendata_process( const sendata_unpacked_t* const sendata );
C_RESULT ardrone_general_sendata_release( void );

#endif //! _ARDRONE_SENDATA_GENERAL_H_

