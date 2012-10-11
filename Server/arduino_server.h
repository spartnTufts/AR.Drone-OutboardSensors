/***********************************************************
 **  arduino_server.h
 **
 **  Server core program to be loaded on to Arduino.
 **  This is the primary header file to be compiled by the
 **  Arduino compiler and used to run the outboard sensor
 **  array.
 **
 **  (C) J.Weeks Tufts University 07/26/2012
 **
 **********************************************************/

#ifndef _ARDUINO_SERVER_
#define _ARDUINO_SERVER_

/**
 * @fn config_wifi
 * @brief loads configuration to transceiver //not functional
 */
void config_wifi(void);

#endif //_ARDUINO_SERVER_
