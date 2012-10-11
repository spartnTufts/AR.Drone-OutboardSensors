/***********************************************************
 ** sendata.c is a modified version of navdata.c
 ** The purpose of this program is to interface with and 
 ** display the information from a third party sensor array
 ** this program may be merged with navdata.c for better
 ** functionality in the future.
 **
 ** J.Weeks Tufts University 08/07/2012
 **********************************************************/

#include <ardrone_tool/Sendata/ardrone_sendata_client.h>
#include <ardrone_tool/Sendata/sendata_common.h>
#include <ardrone_tool/UI/ardrone_input.h>

#include <Sendata/sendata.h>
#include <Navdata/navdata.h>
#include <stdio.h>

uint32_t micros;
uint16_t sec;
uint8_t min;
//arrays for rolling filters
#define FS 5 //filter size
uint16_t sensdat0[FS];
uint16_t sensdat90[FS];
uint16_t sensdat180[FS];
uint16_t sensdat270[FS];
//sensor averages
uint16_t ave0;
uint16_t ave90;
uint16_t ave180;
uint16_t ave270;

uint8_t indx; //array index
int i;
char toggle, msgst, fltmode;
float32_t psi, tgpsi;

/* Initialization local variables before event loop  */
inline C_RESULT demo_sendata_client_init( void* data )
{
  indx=0;
  ave0=0;
  ave90=0;
  ave180=0;
  ave270=0;
  toggle=0;
  msgst=0;
  fltmode=0;
  return C_OK;
}

/* Receving sendata during the event loop */
inline C_RESULT demo_sendata_client_process( const sendata_unpacked_t* const sendata )
{
  const ir_measures_t* ir_m=&sendata->ir_measures;
  const navdata_demo_t* nd;
  if(snav!=NULL) nd=&snav->navdata_demo;
  else nd=NULL;

  //time section
  micros=sendata->sendata_time.time;
  sec=micros>>20;
  micros=micros%1000000;
  min=sec/60;
  sec=sec%60;

  //sensor section
  sensdat0[indx]=ir_m->s0_0;
  sensdat90[indx]=ir_m->s90_0;
  sensdat180[indx]=ir_m->s180_0;
  sensdat270[indx]=ir_m->s270_0;
  indx++;
  if(indx==FS) indx=0; //wrap around

  for(i=0; i<FS; i++) 
    {
      ave0+=sensdat0[i];
      ave90+=sensdat90[i];
      ave180+=sensdat180[i];
      ave270+=sensdat270[i];
    }
  ave0=ave0/FS;
  ave90=ave90/FS;
  ave180=ave180/FS;
  ave270=ave270/FS;

  //control section
  if(ave0>120) toggle=1;
  else toggle=0;

  // em state toggle demo
  /*
  if(toggle==1)
    {
      if(msgst==0) //if message sent 
	{
	  msgst=1;
	  ardrone_tool_set_ui_pad_select(1); //send em toggle command
	}
      else ardrone_tool_set_ui_pad_select(0); //reset em command
    }
  else msgst=0;
  */
  //takeoff land control demo
  /*
  if(toggle==1)
    {
      if(msgst==0) //if message not yet sent
	{
	  msgst=1;
	  fltmode=fltmode^1; //toggle mode
	  ardrone_tool_set_ui_pad_start(fltmode); //send command
	}
    }
  else msgst=0;
  */

  //auto land demo
  /*
  if(toggle==1 && nd!=NULL && nd->altitude>150) //if flying
    {
      ardrone_tool_set_ui_pad_start(0); //send land command
    }
  if(nd!=NULL && nd->altitude<500) ardrone_at_set_progress_cmd(1,0,0,1,0);
  else ardrone_at_set_progress_cmd(0,0,0,0,0);
  */  
  //begin auto flight demo
  /*
  if(nd!=NULL && nd->altitude>150) //only act when airborne
    {
      //land command
      if(ave180>100) ardrone_tool_set_ui_pad_start(0);
  
      //left/right avoidance
      //if(ave90>70) while(ave90>35) ardrone_at_set_progress_cmd(1,(ave90-30)/70,0,0,0);
      //if(ave270>70) while(ave270>35) ardrone_at_set_progress_cmd(1,-(ave270-30)/70,0,0,0);

      //general flight
      if(ave0>140) //rotate left 90deg if object detected 
	{

//	  psi=nd->psi;
	  //psi=psi/1000;
//	  if(psi-90000<-179000) tgpsi=-179000-(psi-90000); //wraparound error
//	  else tgpsi=psi-90000;
//	  while(abs(nd->psi-tgpsi)<1) ardrone_at_set_progress_cmd(1,0,0,0,-1); //rotate ccw

	  i=0;
	  while(i<600)
	    {
	      ardrone_at_set_progress_cmd(1,0,0,0,-1);
	      i++;
	    }
	  }
      else ardrone_at_set_progress_cmd(1,0,-0.01,0,0); //fly forward slowly
    }
*/
  //end auto flight demo

  if(ave0>140) ardrone_at_set_progress_cmd(1,0,-0.05,0,0);
  if(ave180>100) ardrone_at_set_progress_cmd(1,0,0.05,0,0);

  //display section
 	printf("                                                                             \n");
	printf("                                                                             \n");
	printf("                                                                             \n");
	printf("                                                                             \n");
	printf("                                                                             \n");
	printf("===================== Sendata for flight demonstrations =====================\n");

	printf("IR Measurement: s0_0: %i, Ave: %i      s90_0: %i, Ave: %i                    \n", \
	       ir_m->s0_0,ave0,ir_m->s90_0,ave90);
	printf("s180_0: %i, Ave: %i       s270_0: %i, Ave: %i                                \n", \
	       ir_m->s180_0,ave180,ir_m->s270_0,ave270);
	printf("Time          : %i:%i.%i                                                     \n", \
	       min,sec,micros/*sendata->sendata_time.time*/);

       	printf("\033[9A");

  return C_OK;
}

/* Relinquish the local resources after the event loop exit */
inline C_RESULT demo_sendata_client_release( void )
{
  return C_OK;
}

/* Registering to sendata client */
BEGIN_SENDATA_HANDLER_TABLE
  SENDATA_HANDLER_TABLE_ENTRY(demo_sendata_client_init, demo_sendata_client_process, demo_sendata_client_release, NULL)
END_SENDATA_HANDLER_TABLE

