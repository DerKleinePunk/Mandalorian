#pragma once

#if defined(ESP32)
#include <Arduino.h>

#define FPSerial Serial1
#define RXD2 16
#define TXD2 17

extern uint32_t ESP_getFlashChipId(void);

uint32_t ESP_getFlashChipRealSize(void);

void PrintChipInfo();


#endif
