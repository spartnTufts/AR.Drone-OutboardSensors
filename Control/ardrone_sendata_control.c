/***********************************************************
 ** modified version of ardrone_navdata_control.h for the
 ** Sendata program tree.
 **
 ** J.Weeks Tufts University 08/08/2012
 **********************************************************/

#include <ardrone_tool/Control/ardrone_sendata_control.h>
//#include <ardrone_tool/Control/ardrone_control.h>

C_RESULT ardrone_sendata_control_init( void* data )
{
  return C_OK;
}

C_RESULT ardrone_sendata_control_process( const sendata_unpacked_t* const sendata )
{
 /* Signal the client control thread that new sendata arrived.
   */
  return C_OK;
}

C_RESULT ardrone_sendata_control_release( void )
{
  return C_OK;
}
