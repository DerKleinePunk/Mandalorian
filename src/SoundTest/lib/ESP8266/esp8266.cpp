#if defined(ESP8266)
#include "esp8266.hpp"

void PrintChipInfo()
{
    uint32_t realSize = ESP.getFlashChipRealSize();
    uint32_t ideSize = ESP.getFlashChipSize();
    FlashMode_t ideMode = ESP.getFlashChipMode();

    Serial.printf("Reale Flash groesse: %u", realSize);
    Serial.print(" Byte\n\n");
    Serial.printf("In der IDE hinterlegte Flash groesse: %u", ideSize);
    Serial.print(" Byte\n");
    Serial.printf("In der IDE hinterlegte Flash geschwindigkeit: %u\n", ESP.getFlashChipSpeed());
    Serial.printf("In der IDE hinterlegter  Flash Modus:  %s\n\n",
                  (ideMode == FM_QIO  ? "QIO" :
                   ideMode == FM_QOUT ? "QOUT" :
                   ideMode == FM_DIO  ? "DIO" :
                   ideMode == FM_DOUT ? "DOUT" :
                                        "UNKNOWN"));
    if(ideSize != realSize) {
        Serial.println("Flash konfiguration ist falsch!\n");
    } else {
        Serial.println("Flash konfigguration ist korrekt.\n");
    }

    Serial.printf("Reale Flash ID:   %08X\n", ESP.getFlashChipId());
}

#endif
