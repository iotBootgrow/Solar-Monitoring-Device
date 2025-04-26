{\rtf1\ansi\ansicpg1252\cocoartf2822
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fswiss\fcharset0 Helvetica;}
{\colortbl;\red255\green255\blue255;}
{\*\expandedcolortbl;;}
\margl1440\margr1440\vieww11520\viewh8400\viewkind0
\pard\tx720\tx1440\tx2160\tx2880\tx3600\tx4320\tx5040\tx5760\tx6480\tx7200\tx7920\tx8640\pardirnatural\partightenfactor0

\f0\fs24 \cf0 #ifndef LCDMANAGER_H\
#define LCDMANAGER_H\
\
// Display page function already included in main .ino (displayPage function).\
// This header manages the page counter if needed for future expansions.\
\
extern int currentPage;\
extern const int totalPages;\
extern unsigned long lastScrollTime;\
\
// Manage Page Change Timing (Optional if needed separately)\
void handlePageChange() \{\
  if (millis() - lastScrollTime > 5000) \{ // Every 5 seconds\
    lastScrollTime = millis();\
    currentPage = (currentPage + 1) % totalPages;\
  \}\
\}\
\
#endif}