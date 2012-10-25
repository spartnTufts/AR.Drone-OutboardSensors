/***********************************************************
 **  sendata.c
 **
 **  This is a modified version of navdata.c rewritten to
 **  interface with and display the information from a third 
 **  party sensor array.
 **
 **  (C) J.Weeks Tufts University 08/07/2012
 **
 **  navdata.c (C) PARROT SA 2007-2011
 **
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
int i, c;
char toggle, msgst, fltmode;

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
  c=0;
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

  //reset averages
  ave0=0;
  ave90=0;
  ave180=0;
  ave270=0;

  //create averages
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



  // em state toggle demo - flip emergency state based on senosor reading
  /*
  if(ave0>120) toggle=1;
  else toggle=0;

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
  //takeoff land control demo - takeoff and land based on sensor reading
  /*
  if(ave0>120) toggle=1;
  else toggle=0;

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

  //auto land demo - land based on sensor reading
  /*
  if(ave0>120) toggle=1;
  else toggle=0;

  if(toggle==1 && nd!=NULL && nd->altitude>150) //if flying
    {
      ardrone_tool_set_ui_pad_start(0); //send land command
    }
  if(nd!=NULL && nd->altitude<500) ardrone_at_set_progress_cmd(1,0,0,1,0);
  else ardrone_at_set_progress_cmd(0,0,0,0,0);
  */  
  //begin auto flight demo -- hull on
  /* 
  if(nd!=NULL && nd->altitude>150) //only act when airborne
    {
      //land command
      //if(ave180>100) ardrone_tool_set_ui_pad_start(0);
  
      //left/right avoidance
      //if(ave90>200) while(ave90>150) ardrone_at_set_progress_cmd(1,(ave90-150)/100,0,0.7,0);
      //if(ave270>200) while(ave270>150) ardrone_at_set_progress_cmd(1,-(ave270-150)/100,0,0.7,0);

      //general flight
      if(ave0>80 && (c==0 || c<0)) //rotate right 90deg if front object detected 
	{
	  c=100; //set rotation counter
	}

      if(c>0) //rotate if c>0
	{
	  ardrone_at_set_progress_cmd(1,0,0,0,-0.75);
	  c--;
	}
      if(c<11 && c>0) //restore altitude
	{
	  //ardrone_at_set_progress_cmd(1,0,0,1,0);
	  c--;
	}
      else ardrone_at_set_progress_cmd(1,-0.025,-0.02,0.70,0); //fly forward slowly
    }
    //end auto filght demo -- hull on
  */  

  //begin auto flight demo -- no hull
  if(ave0>100) //rotate left 90deg if front object detected
    {
      ardrone_at_set_progress_cmd(1,0,0,0,-0.75);
    }
  else ardrone_at_set_progress_cmd(0,0,0,0,0);
  //end auto flight demo -- no hull

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
	printf("Time          : %i:%i.%i      Rotation Counter: %i                           \n", \
	       min,sec,micros,c/*sendata->sendata_time.time*/);

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

