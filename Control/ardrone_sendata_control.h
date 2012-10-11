/***********************************************************
 ** modified version of ardrone_navdata_control.h for the
 ** Sendata program tree.
 **
 ** J.Weeks Tufts University 08/08/2012
 **********************************************************/

#ifndef _ARDRONE_SENDATA_CONTROL_H_
#define _ARDRONE_SENDATA_CONTROL_H_

#include <ardrone_tool/Sendata/ardrone_sendata_client.h>

//
// Handler that resume ardrone control thread when new sendatas are received
//

C_RESULT ardrone_sendata_control_init( void* data );
C_RESULT ardrone_sendata_control_process( const sendata_unpacked_t* const sendata );
C_RESULT ardrone_sendata_control_release( void );

#endif // _ARDRONE_SENDATA_CONTROL_H_
