#pragma once

#if defined(ESP8266) // Using a soft serial port
#include <Arduino.h>
#include <SoftwareSerial.h>
SoftwareSerial softSerial(/*rx =*/D0, /*tx =*/D1);
#define FPSerial softSerial


#endif
