/***********************************************************
 ** arduino_server.cpp
 **
 ** Server core program to be loaded on to Arduino.
 ** This is the primary file to be compiled by the
 ** Arduino compiler and used to run the outboard
 ** sensor array.
 **
 ** (C) J.Weeks Tufts University 07/26/2012
 **
 **********************************************************/

#define _ARDUINO_
#include <arduino_server.h>
#include <sendata_server.h>
#include <sendata_common.h>
#include <sendata_config.h>
#include <Arduino.h>
#include <stdint.h>

//constant declarations
#define WIFI_SERIAL Serial3
#define WIFI_BAUD 9600
#define WIFI_RST_PIN 40
#define MATCHCHAR '#'
#define S0_0_PIN A0
#define S90_0_PIN A1
#define S180_0_PIN A2
#define S270_0_PIN A3


//variable declarations
sendatas_t sendatas;
static uint8_t buff_head[SENDATA_MAX_SIZE];
uint8_t* buff_tail;
uint16_t* s0_0p;
uint16_t* s90_0p;
uint16_t* s180_0p;
uint16_t* s270_0p;

void setup(void)
{
  delay(30000); //wait for drone to come on-line
  //Serial.begin(9600);  //Debug
  WIFI_SERIAL.begin(WIFI_BAUD);
  pinMode(WIFI_RST_PIN, OUTPUT);
  initialize(&sendatas);

  //reboot wifi chip
  digitalWrite(WIFI_RST_PIN, LOW);
  delay(1000);
  digitalWrite(WIFI_RST_PIN, HIGH);
  delay(500);

  //config_wifi();
  //initialize sensor vars
  s0_0p=&(sendatas.ir_measures.s0_0);
  *s0_0p=0;
  s90_0p=&(sendatas.ir_measures.s90_0);
  *s90_0p=0;
  s180_0p=&(sendatas.ir_measures.s180_0);
  *s180_0p=0;
  s270_0p=&(sendatas.ir_measures.s270_0);
  *s270_0p=0;
}

void loop(void)
{
  //read sensor values
  *s0_0p=analogRead(S0_0_PIN);
  *s90_0p=analogRead(S90_0_PIN);
  *s180_0p=analogRead(S180_0_PIN);
  *s270_0p=analogRead(S270_0_PIN);
  //always last part of loop
  sendatas.sendata_time.time=micros();
  buff_tail=pack(buff_head, &sendatas);
  WIFI_SERIAL.flush();
  WIFI_SERIAL.write(buff_head, buff_tail-buff_head);
  WIFI_SERIAL.flush();
  WIFI_SERIAL.print(MATCHCHAR);
  //Debug
  //Serial.write(buff_head, buff_tail-buff_head);
  //Serial.println();
}


// /**
//  * @fn serialEvent3
//  * @brief debugging for wifi chip, forwards data to terminal
//  */
// void serialEvent3()
// {
//   while(Serial3.available()>0)
//     {
//       Serial.write(Serial3.read());
//     }
// }


/**
 * @fn config_wifi
 * @brief loads wifi configuration to transceiver -- not functional
 */
void config_wifi(void)
{
//   //hard reboot chip
//   delay(500);
//   WIFI_SERIAL.flush();
//   //Serial.println("chip reboot");
//   pinMode(WIFI_RST_PIN, OUTPUT);
//   delay(1000);
//   pinMode(WIFI_RST_PIN, INPUT);
//   //Serial.println("waiting for chip to become ready");
//   while(!WIFI_SERIAL.available()); //wait for chip to become ready
//   delay(2000); //let buffer
//   //while(WIFI_SERIAL.available()>0) {
//   //  Serial.write(WIFI_SERIAL.read());
//   //}

//   //load config
//   //  Serial.println("load config");
//   WIFI_SERIAL.print("$$$");
//   WIFI_SERIAL.flush(); 
//   delay(500);
//   WIFI_SERIAL.print("set wlan ssid ardx");
//   WIFI_SERIAL.flush(); 
//   WIFI_SERIAL.print('\r');
//   WIFI_SERIAL.print("set ip proto 1");
//   WIFI_SERIAL.flush(); 
//   WIFI_SERIAL.print('\r');
//   WIFI_SERIAL.print("set ip host ");
//   WIFI_SERIAL.print(WIFI_MOBILE_IP);
//   WIFI_SERIAL.flush(); 
//   WIFI_SERIAL.print('\r');
//   WIFI_SERIAL.print("set ip ");
//   WIFI_SERIAL.print(WIFI_ARDUINO_IP);
//   WIFI_SERIAL.flush(); 
//   WIFI_SERIAL.print('\r');
//   WIFI_SERIAL.print("set ip remote ");
//   WIFI_SERIAL.print(SENDATA_PORT);
//   WIFI_SERIAL.flush(); 
//   WIFI_SERIAL.print('\r');
//   WIFI_SERIAL.print("set ip local ");
//   WIFI_SERIAL.print(SENDATA_PORT);
//   WIFI_SERIAL.flush(); 
//   WIFI_SERIAL.print('\r');
//   //save config
//   WIFI_SERIAL.print("save");
//   WIFI_SERIAL.flush(); 
//   WIFI_SERIAL.print('\r');
//   //soft reboot chip
//   WIFI_SERIAL.print("reboot");
//   WIFI_SERIAL.flush(); 
//   WIFI_SERIAL.print('\r');
//   //Serial.println("waiting for chip to become ready");
//   while(!WIFI_SERIAL.available()); //wait for chip to become ready
//   delay(1000);  
// //while(WIFI_SERIAL.available()>0) {
//   //  Serial.write(WIFI_SERIAL.read());
//   //}

//   //DEBUG: print settings to serial
//   /*Serial.println("confirm config");
//   WIFI_SERIAL.print("$$$");
//   delay(500);
//   WIFI_SERIAL.print("get everything");
//   WIFI_SERIAL.flush();
//   WIFI_SERIAL.print('\r');
//   WIFI_SERIAL.print("exit\r");
//   WIFI_SERIAL.flush(); 
//   */
}
