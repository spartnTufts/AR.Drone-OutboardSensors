/***********************************************************
 **  sendata_server.h
 **
 **  Server for Arduino responsible for initialization and 
 **  packing of sendata for transmission to transceiver.
 **
 **  (C) J.Weeks Tufts University 07/25/2012
 **
 **********************************************************/

#ifndef _SENDATA_SERVER_H_
#define _SENDATA_SERVER_H_
#ifdef _ARDUINO_

#include <sendata_common.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct _sendatas_t
 * @brief Wrapper struct for sendata structures
 */
typedef struct _sendatas_t
{
  sendata_t sendata;
  //include all structures to be used/sent to client
  ir_measures_t ir_measures;
  sendata_time_t sendata_time;
  sendata_cks_t sendata_cks;
} sendatas_t;

/**
 * @fn initialize
 * @brief initializes sendata_t contents
 * @pram sendatas Sendatas to be initialized
 */
void initialize(sendatas_t* sendatas);

/**
 * @fn pack
 * @brief packs sendatas for UDP transmission
 * @param sendata_ptr Pointer to start of transmission buffer
 * @param sendatas Sendatas to be transmitted
 * @ret pointer to end of transmission buffer
 */
uint8_t* pack(uint8_t* sendata_pointer, sendatas_t* sendatas);

#ifdef __cplusplus
}
#endif

#endif //_ARDUINO_
#endif //_SENDATA_SERVER_H_
