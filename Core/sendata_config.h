/***********************************************************
 **  sendata_config.h
 **
 **  File to store user variables similar to config.makefile
 **  and config.h.  This file is used to provide a central 
 **  store and avoid the need to modify multiple makefiles 
 **  and/or set make flags.
 **
 **  (C) J.Weeks Tufts University 07/19/2012
 **
 **  config.makefile config.h (C) PARROT SA 2007-2011
 **
 **********************************************************/

#ifndef _SENDATA_CONFIG_H_
#define _SENDATA_CONFIG_H_

#define SENDATA_PORT 5574

#define WIFI_ARDUINO_IP "192.168.1.3"

#ifdef _ARDUINO_
#define WIFI_MOBILE_IP "192.168.1.2"
#endif

#define SENDATA_SEQUENCE_DEFAULT  1

#define SENDATA_HEADER  0x88776655

#define SENDATA_MAX_SIZE 2048
#define SENDATA_MAX_CUSTOM_TIME_SAVE 20

#endif //_SENDATA_CONFIG_H_
