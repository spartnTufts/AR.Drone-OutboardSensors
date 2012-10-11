/***********************************************************
 **  sendata.c
 **
 **  This is a modified version of navdata.c rewritten to 
 **  accomidate data input from an outboard sensor array.
 **
 **  (C) J.Weeks Tufts University 07/18/2012
 **
 **  navdata.c (C) PARROT SA 2007-2011
 **
 ***********************************************************/

#ifndef _ARDUINO_
#include <VP_Os/vp_os_types.h>
#include <VP_Os/vp_os_malloc.h>
#include <VP_Os/vp_os_print.h>

#include <ardrone_api.h>
#include <ardrone_tool/Sendata/sendata.h>

#else
//#include <stdint.h>
#include <sendata.h>
#endif

/* Uncomment to activate interesting printf's */
//#define DEBUG_SENDATA_C

/**
 * @fn ardrone_sendata_compute_cks:
 * @param nv Data to calculate the checksum.
 * @param size Size of data calculate as follow : size-sizeof(sendata_cks_t).
 * @return Retrieves the checksum from the sendata nv.
 */
uint32_t ardrone_sendata_compute_cks( uint8_t* nv, int32_t size )
{
  int32_t i;
  uint32_t cks;
  uint32_t temp;

  cks = 0;

  for( i = 0; i < size; i++ )
  {
    temp = nv[i];
    cks += temp;
  }

  return cks;
}

/**
 * @fn ardrone_sendata_search_option:
 * @param sendata_options_ptr
 * @param tag ID of the bloc to search for.
 * @brief Jumps to a specified 'option' (block of sendata) inside
 * a sendata packed buffer.
 */
sendata_option_t* ardrone_sendata_search_option( sendata_option_t* sendata_options_ptr, sendata_tag_t tag )
{
  uint8_t* ptr;

  while( sendata_options_ptr->tag != tag )
  {
    ptr  = (uint8_t*) sendata_options_ptr;
    ptr += sendata_options_ptr->size;

    sendata_options_ptr = (sendata_option_t*) ptr;
  }

  return sendata_options_ptr;
}

#ifndef _ARDUINO_
/**
 * @fn ardrone_sendata_unpack_all:
 * @param sendata_unpacked  sendata_unpacked in which to store the sendata.
 * @param sendata One packet read from the port SENDATA.
 * @param Checksum of sendata
 * @brief Disassembles a buffer of received sendata, and dispatches
 * it inside 'sendata_unpacked' structure.
 */
C_RESULT ardrone_sendata_unpack_all(sendata_unpacked_t* sendata_unpacked, sendata_t* sendata, uint32_t* cks)
{
  C_RESULT res;
  sendata_cks_t sendata_cks = { 0 };
  sendata_option_t* sendata_option_ptr;

  sendata_option_ptr = (sendata_option_t*) &sendata->options[0];

  vp_os_memset( sendata_unpacked, 0, sizeof(*sendata_unpacked) );

  res = C_OK;

#ifdef DEBUG_SENDATA_C
  printf("Received sendatas tags :");
#endif


  while( sendata_option_ptr != NULL )
    {
      // Check if we have a valid option
      if( sendata_option_ptr->size == 0 )
	{
	  PRINT("One sendata option (%d) is not a valid option because its size is zero\n", sendata_option_ptr->tag);
	  sendata_option_ptr = NULL;
	  res = C_FAIL;
	}
      else
	{
	  
	  if( sendata_option_ptr->tag <= SENDATA_NUM_TAGS){
#ifdef DEBUG_SENDATA_C
	    printf("[%d]",sendata_option_ptr->tag);
#endif
	    sendata_unpacked->last_sendata_refresh |= SENDATA_OPTION_MASK(sendata_option_ptr->tag);
	  }

	  switch( sendata_option_ptr->tag )
	    {
	      
#define SENDATA_OPTION(STRUCTURE,NAME,TAG)	\
	      case TAG:							\
		sendata_option_ptr = ardrone_sendata_unpack( sendata_option_ptr, sendata_unpacked->NAME); \
		break;

#define SENDATA_OPTION_DEMO(STRUCTURE,NAME,TAG)  SENDATA_OPTION(STRUCTURE,NAME,TAG)
#define SENDATA_OPTION_CKS(STRUCTURE,NAME,TAG) {}
	      
#include <ardrone_tool/Sendata/sendata_keys.h>

	    case SENDATA_CKS_TAG:
	      sendata_option_ptr = ardrone_sendata_unpack( sendata_option_ptr, sendata_cks );
	      *cks = sendata_cks.cks;
	      sendata_option_ptr = NULL; // End of structure
	      break;

	    default:
	      PRINT("Tag %d is an unknown sendata option tag\n", (int) sendata_option_ptr->tag);
	      sendata_option_ptr = (sendata_option_t *)(((uint32_t)sendata_option_ptr) + sendata_option_ptr->size);
	      break;
	    }
	}
    }

#ifdef DEBUG_SENDATA_C
  printf("\n");
#endif

  return res;
}

#endif //_ARDUINO_
