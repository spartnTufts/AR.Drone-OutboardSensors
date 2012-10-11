/***********************************************************
 **  sendata_server.c
 **
 **  Server for Arduino responsible for initialization and 
 **  packing of sendata for transmission to transceiver.
 **
 **  (C) J.Weeks Tufts University 07/25/2012
 **
 **********************************************************/

#include <sendata.h>
#include <sendata_server.h>
#include <sendata_common.h>
#include <sendata_config.h>

/**
 * @fn initialize
 * @brief initializes sendata_t contents
 * @pram sendatas Sendatas to be initialized
 */
void initialize(sendatas_t* sendatas)
{
  sendatas->sendata.header=SENDATA_HEADER;
  sendatas->sendata.sequence=SENDATA_SEQUENCE_DEFAULT;
  sendatas->ir_measures.tag=IR_MEASURES_TAG;
  sendatas->ir_measures.size=sizeof(ir_measures_t);
  sendatas->sendata_time.tag=SENDATA_TIME_TAG;
  sendatas->sendata_time.size=sizeof(sendata_time_t);
  sendatas->sendata_cks.tag=SENDATA_CKS_TAG;
  sendatas->sendata_cks.size=sizeof(sendata_cks_t);
}

/**
 * @fn pack
 * @brief packs sendatas for UDP transmission
 * @pram sendata_ptr Pointer to start of transmission buffer
 * @param sendatas Sendatas to be transmitted
 */
uint8_t* pack(uint8_t* sendata_pointer, sendatas_t* sendatas)
{
  uint8_t* traveler=sendata_pointer;
  //load sendata into buffer
  sendatas->sendata.sequence++; //set sequence to next
  traveler=sendata_pack_option(traveler, (uint8_t*) &sendatas->sendata, sizeof(sendatas->sendata));
  traveler-=sizeof(sendata_option_t);
  //load rest of sendatas
  traveler=(uint8_t*) ardrone_sendata_pack(traveler, sendatas->sendata_time);
  traveler=(uint8_t*) ardrone_sendata_pack(traveler, sendatas->ir_measures);
  //compute and store checksum
  sendatas->sendata_cks.cks=ardrone_sendata_compute_cks(sendata_pointer, traveler-sendata_pointer);
  //bugfix add 255 to checksum
  sendatas->sendata_cks.cks+=255;
  //load cks_t to buffer
  traveler=(uint8_t*) ardrone_sendata_pack(traveler, sendatas->sendata_cks);
  return traveler;
}
