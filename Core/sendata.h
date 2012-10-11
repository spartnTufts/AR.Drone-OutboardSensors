/***********************************************************
 **  sendata.h
 **
 **  This is a modified version of a subset of ardrone_api.h
 **  specificaly the section labled NAVDATA FUNCTIONS 
 **  rewritten to accomidate data input from an outboard 
 **  sensor array.
 **  
 **  (C) J.Weeks Tufts University 07/18/2012
 **
 **  ardrone_api.h (C) PARROT SA 2007-2011
 **
 ***********************************************************/

#ifndef _SENDATA_H_
#define _SENDATA_H_

#ifndef _ARDUINO_
#include <ardrone_api.h>
//#include <ardrone_common_config.h>
//#include <ATcodec/ATcodec_api.h>
//#include <navdata_common.h>
//#include <vision_common.h>
#include <VP_Os/vp_os_malloc.h>
//#include <Maths/quaternions.h>
#include <ardrone_tool/Sendata/sendata_common.h>
#include <ardrone_tool/Sendata/sendata_config.h>
#else
//#include <stdint.h>
#include <sendata_common.h>
#include <sendata_config.h>
#include <string.h>
//from parrot sdk
#define vp_os_memcpy memcpy
#define INLINE __inline__ __attribute__((always_inline))
#define WEAK __attribute__((weak))
#define API_WEAK WEAK
#define C_RESULT int
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct _sendata_unpacked_t
 * @brief Decoded outboard sensor data.
*/
typedef struct _sendata_unpacked_t
{
  uint32_t  last_sendata_refresh;  
  //mask showing which block was refreshed when receiving sendata

#define SENDATA_OPTION(STRUCTURE,NAME,TAG)       STRUCTURE NAME ;
#define SENDATA_OPTION_CKS(STRUCTURE,NAME,TAG)
#ifndef _ARDUINO_	
#include <ardrone_tool/Sendata/sendata_keys.h>
#else
#include <sendata_keys.h>
#endif

} sendata_unpacked_t;

/**
 * @def ardrone_sendata_pack
 * @brief Add an 'option' to the sendata network packet to be sent to a client.
 * Used by the drone 'navdata' thread.
*/
#define ardrone_sendata_pack( sendata_ptr, option ) (sendata_option_t*) \
  sendata_pack_option( (uint8_t*) sendata_ptr, (uint8_t*) &option, option.size )

/**
 * @def ardrone_sendata_unpack
 * @brief Extract an'option' from the sendata network packet sent by the arduino.
 * Used by the client 'sendata' thread inside ARDroneTool.
*/
#define ardrone_sendata_unpack( sendata_ptr, option ) (sendata_option_t*) \
  sendata_unpack_option( (uint8_t*) sendata_ptr, sendata_ptr->size, \
			 (uint8_t*) &option, sizeof (option) )

/**
 * @brief Add an 'option' to the sendata network packet to be sent to a client.
 * Used by the drone 'navdata' thread.
 * @param sendata_ptr Pointer to the buffer containing packed sendata to be sent.
*/
static INLINE uint8_t* sendata_pack_option( uint8_t* sendata_ptr, uint8_t* data, uint32_t size )
{
  vp_os_memcpy(sendata_ptr, data, size);
  return (sendata_ptr + size);
}

/**
 * @fn sendata_unpack_option
 * @brief Extract an 'option' from the sendata network packet sent by the arduino.
 * Used by the client 'sendata' thread inside ARDroneTool.
*/
static INLINE uint8_t* sendata_unpack_option( uint8_t* sendata_ptr, uint32_t ptrsize, \
					      uint8_t* data, uint32_t datasize )
{
  uint32_t minSize = (ptrsize < datasize) ? ptrsize : datasize;
  vp_os_memcpy(data, sendata_ptr, minSize);
  return (sendata_ptr + ptrsize);
}

/**
 * @fn sendata_next_option
 * @brief Jumps to the next 'option' inside the packed sendata.
 * Used by the client 'sendata' thread inside ARDroneTool.
*/
static INLINE sendata_option_t* sendata_next_option( sendata_option_t* sendata_options_ptr )
{
  uint8_t* ptr;

  ptr  = (uint8_t*) sendata_options_ptr;
  ptr += sendata_options_ptr->size;

  return (sendata_option_t*) ptr;
}

/**
 * @brief Creates a checksum from a packed sendata packet.
 * @param nv Data to calculate the checksum.
 * @param size Size of data calculate as follow : size-sizeof(sendata_cks_t).
 * @return Retrieves the checksum from the sendata nv.
 */
uint32_t ardrone_sendata_compute_cks( uint8_t* nv, int32_t size ) API_WEAK;

/**
 * @param sendata_unpacked  sendata_unpacked in which to store the sendata.
 * @param sendata One packet read from the port SENDATA.
 * @param Checksum of sendata
 * @brief Disassembles a buffer of received sendata, and dispatches it inside 'sendata_unpacked' structure.
 */
C_RESULT ardrone_sendata_unpack_all(sendata_unpacked_t* sendata_unpacked, sendata_t* sendata, \
				    uint32_t* cks) API_WEAK;

/**
 * @param sendata_options_ptr
 * @param Tag ID of the bloc to search for.
 * @brief Jumps to a specified 'option' (block of sendata) inside a sendata packed buffer.
 */
sendata_option_t* ardrone_sendata_search_option( sendata_option_t* sendata_options_ptr, \
						 sendata_tag_t tag ) API_WEAK;

#ifdef __cplusplus
}
#endif

#endif //_SENDATA_H_
