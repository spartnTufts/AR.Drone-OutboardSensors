/***********************************************************
 **  sendata_common.h
 **
 **  This is a modified version of navdata_common.h 
 **  rewritten to accommodate data input from an outboard 
 **  sensor array.
 ** 
 **  (C) J.Weeks Tufts University 07/18/2012
 **
 **  navdata_common.h (C) PARROT SA 2007-2011
 **
 ***********************************************************/

#ifndef _SENDATA_COMMON_H_
#define _SENDATA_COMMON_H_

#ifdef _ARDUINO_
#include <stdint.h>
#endif

#ifndef _ATTRIBUTE_PACKED_
  #if defined(_MSC_VER)
    #define _ATTRIBUTE_PACKED_
    //Asks Visual C++ to pack structures from now on
    #pragma pack(1)
  #else
    #define _ATTRIBUTE_PACKED_  __attribute__ ((packed))
  #endif
#endif

/**
 * @brief Tags identifying sendata blocks in a Sendata UDP packet
 * This tag is stored in the first two bytes of any sendata block (aka 'option').
 */

#define SENDATA_OPTION(STRUCTURE,NAME,TAG)       TAG ,
#define SENDATA_OPTION_CKS(STRUCTURE,NAME,TAG)   SENDATA_NUM_TAGS, TAG = 0xFFFF

typedef enum _sendata_tag_t {
#ifndef _ARDUINO_
  #include <ardrone_tool/Sendata/sendata_keys.h>
#else
  #include <sendata_keys.h>
#endif
} sendata_tag_t;

#define SENDATA_OPTION_MASK(option) ( 1 << (option) )
#define SENDATA_OPTION_FULL_MASK    ((1<<SENDATA_NUM_TAGS)-1)

typedef struct _sendata_option_t {
  uint16_t  tag;
  uint16_t  size;
#if defined _MSC_VER
  uint8_t   *data;
#else
  uint8_t   data[];
#endif
} sendata_option_t;

/**
 * @brief Sendata structure sent over the network.
 */
typedef struct _sendata_t {
  uint32_t    header;		//Always set to SENDATA_HEADER
  uint32_t    sequence;         //Sequence number, incremented for each sent packet
 
  sendata_option_t  options[1];
}_ATTRIBUTE_PACKED_ sendata_t;


//All sendata options can be extended (new values AT THE END).
//New sendata options may be added.

/**
 * @brief Last sendata option that *must* be included at the end of all sendata packets
 * + 6 bytes
 */
typedef struct _sendata_cks_t {
  uint16_t  tag;
  uint16_t  size;

  // Checksum for all sendatas (including options)
  uint32_t  cks;
}_ATTRIBUTE_PACKED_ sendata_cks_t;

/**
 * @brief Timestamp
 * + 6 bytes
 */
typedef struct _sendata_time_t {
  uint16_t  tag;
  uint16_t  size;

  uint32_t  time;  //32 bit value where the 11 most significant bits represents the seconds, and the 21 least significant bits are the microseconds.
}_ATTRIBUTE_PACKED_ sendata_time_t;

/**
 * @brief IR sensor measurements
 * @nominclature x_y are horizontal cw and vertical "up first" degrees 
 * ie 0_90 is straight up, 90_0 is dead starboard
 */
typedef struct _ir_measures_t {
  uint16_t  tag;
  uint16_t  size;

  uint16_t  s0_0;
  uint16_t  s90_0;
  uint16_t  s180_0;
  uint16_t  s270_0;

} _ATTRIBUTE_PACKED_ ir_measures_t;

typedef struct _sendata_watchdog_t {
  uint16_t   tag;
  uint16_t   size;

  // +4 bytes
  int32_t    watchdog;
}_ATTRIBUTE_PACKED_ sendata_watchdog_t;

#if defined(_MSC_VER)
  //Go back to default packing policy
  #pragma pack()
#endif

#endif // _SENDATA_COMMON_H_
