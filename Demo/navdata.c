#include <ardrone_tool/Navdata/ardrone_navdata_client.h>

#include <Navdata/navdata.h>
#include <stdio.h>

/* Initialization local variables before event loop  */
inline C_RESULT demo_navdata_client_init( void* data )
{
  return C_OK;
}

/* Receving navdata during the event loop */
inline C_RESULT demo_navdata_client_process( const navdata_unpacked_t* const navdata )
{
	const navdata_demo_t*nd = &navdata->navdata_demo;
	snav=navdata;//make navdata accessible to sendata thread

	printf("                                                                             \n");
	printf("===================== Navdata for flight demonstrations =====================\n");

	printf("Control state : %i  Battery level : %i mV  Altitude : %i         \n", \
	       nd->ctrl_state,nd->vbat_flying_percentage,nd->altitude);
	printf("Orientation   : [Theta] %4.3f  [Phi] %4.3f  [Psi] %4.3f           \n", \
	       nd->theta,nd->phi,nd->psi);
	printf("Speed         : [vX] %4.3f  [vY] %4.3f  [vZ] %4.3f                \n", \
	       nd->vx,nd->vy,nd->vz);
	
	printf("\n\n\n\n");
	printf("\033[9A");

  return C_OK;
}

/* Relinquish the local resources after the event loop exit */
inline C_RESULT demo_navdata_client_release( void )
{
  return C_OK;
}

/* Registering to navdata client */
BEGIN_NAVDATA_HANDLER_TABLE
  NAVDATA_HANDLER_TABLE_ENTRY(demo_navdata_client_init, demo_navdata_client_process, demo_navdata_client_release, NULL)
END_NAVDATA_HANDLER_TABLE

