{\rtf1\ansi\ansicpg1252\cocoartf2822
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fswiss\fcharset0 Helvetica;}
{\colortbl;\red255\green255\blue255;}
{\*\expandedcolortbl;;}
\margl1440\margr1440\vieww11520\viewh8400\viewkind0
\pard\tx720\tx1440\tx2160\tx2880\tx3600\tx4320\tx5040\tx5760\tx6480\tx7200\tx7920\tx8640\pardirnatural\partightenfactor0

\f0\fs24 \cf0 #ifndef INVERTERSETTINGS_H\
#define INVERTERSETTINGS_H\
\
#include <Preferences.h>\
\
Preferences preferences;\
\
// Save detected baud rate and slave ID to Flash\
void saveSettings(int baudRate, int slaveID) \{\
  preferences.begin("solar_config", false);\
  preferences.putInt("baudrate", baudRate);\
  preferences.putInt("slaveid", slaveID);\
  preferences.end();\
  Serial.println("Settings Saved: Baud " + String(baudRate) + " | Slave ID " + String(slaveID));\
\}\
\
// Load baud rate and slave ID from Flash\
void loadSettings() \{\
  preferences.begin("solar_config", true);\
  detectedBaudRate = preferences.getInt("baudrate", 0);\
  detectedSlaveID = preferences.getInt("slaveid", 1); // Default slave ID 1\
  preferences.end();\
  \
  Serial.println("Settings Loaded: Baud " + String(detectedBaudRate) + " | Slave ID " + String(detectedSlaveID));\
\}\
\
#endif}