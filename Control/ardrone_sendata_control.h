/***********************************************************
 **  ardrone_sendata_control.h 
 **
 **  This is a modified version of ardrone_navdata_control.h
 **  rewritten for the Sendata program tree.
 **
 **  (C) J.Weeks Tufts University 08/08/2012
 **
 **  ardrone_navdata_control.h (C) PARROT SA 2007-2011
 **
 **********************************************************/

#ifndef _ARDRONE_SENDATA_CONTROL_H_
#define _ARDRONE_SENDATA_CONTROL_H_

#include <ardrone_tool/Sendata/ardrone_sendata_client.h>

//
// Handler that resumes ardrone control thread when new sendatas are received
//

C_RESULT ardrone_sendata_control_init( void* data );
C_RESULT ardrone_sendata_control_process( const sendata_unpacked_t* const sendata );
C_RESULT ardrone_sendata_control_release( void );

#endif // _ARDRONE_SENDATA_CONTROL_H_
