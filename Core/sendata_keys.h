/***********************************************************
 **  sendata_keys.h
 **
 **  This is a modified version of navdata_keys.h rewritten  
 **  to accomidate data input from an outboard sensor array.
 **  
 **  (C) J.Weeks Tufts University 07/18/2012
 **
 **  navdata_keys.h (C) PARROT SA 2007-2011
 **
 ***********************************************************/

#ifndef SENDATA_OPTION
	#define SENDATA_OPTION(x,y,z)
#endif
#ifndef SENDATA_OPTION_CKS
	#define SENDATA_OPTION_CKS(x,y,z)
#endif

SENDATA_OPTION( ir_measures_t,          ir_measures           , IR_MEASURES_TAG           )
SENDATA_OPTION( sendata_time_t,         sendata_time          , SENDATA_TIME_TAG          )
SENDATA_OPTION( sendata_watchdog_t,     sendata_watchdog      , SENDATA_WATCHDOG_TAG      )

SENDATA_OPTION_CKS( sendata_cks_t,      sendata_cks           , SENDATA_CKS_TAG           )


#undef SENDATA_OPTION
#undef SENDATA_OPTION_CKS
