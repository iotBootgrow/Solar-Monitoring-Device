{\rtf1\ansi\ansicpg1252\cocoartf2822
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fswiss\fcharset0 Helvetica;}
{\colortbl;\red255\green255\blue255;}
{\*\expandedcolortbl;;}
\margl1440\margr1440\vieww11520\viewh8400\viewkind0
\pard\tx720\tx1440\tx2160\tx2880\tx3600\tx4320\tx5040\tx5760\tx6480\tx7200\tx7920\tx8640\pardirnatural\partightenfactor0

\f0\fs24 \cf0 #ifndef AUTOBAUDSCANNER_H\
#define AUTOBAUDSCANNER_H\
\
#include <HardwareSerial.h>\
\
HardwareSerial RS485Serial(2); // Using UART2 for RS485\
\
int autoBaudDetect() \{\
  int baudRates[] = \{9600, 19200, 38400, 57600, 115200\};\
  int numRates = sizeof(baudRates) / sizeof(baudRates[0]);\
\
  Serial.println("Starting Auto Baud Detection...");\
\
  for (int i = 0; i < numRates; i++) \{\
    Serial.println("Trying baud rate: " + String(baudRates[i]));\
    RS485Serial.begin(baudRates[i], SERIAL_8N1, MODEM_RX, MODEM_TX);\
    delay(500);\
\
    // Send a simple Modbus Read command or ping (Dummy attempt)\
    RS485Serial.write(0x01); // Assuming Slave ID 1\
    RS485Serial.write(0x03); // Function Code (Read Holding Registers)\
    RS485Serial.write(0x00);\
    RS485Serial.write(0x00);\
    RS485Serial.write(0x00);\
    RS485Serial.write(0x01);\
    RS485Serial.write(0x84);\
    RS485Serial.write(0x0A);\
    delay(200);\
\
    if (RS485Serial.available()) \{\
      Serial.println("Inverter Responded at baud: " + String(baudRates[i]));\
      RS485Serial.end(); // Close temporary RS485 connection\
      return baudRates[i];\
    \}\
    \
    RS485Serial.end(); // Close if no response\
    delay(500);\
  \}\
\
  Serial.println("Auto Baud Detection Failed. Defaulting to 9600.");\
  return 9600; // Default fallback\
\}\
\
#endif}