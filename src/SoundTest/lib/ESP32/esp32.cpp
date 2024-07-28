#if(defined(ESP32))

#include "esp32.hpp"


uint32_t ESP_getFlashChipRealSize(void)
{
    uint32_t id = (ESP_getFlashChipId() >> 16) & 0xFF;
    return 2 << (id - 1);
}

void PrintChipInfo()
{
    uint32_t realSize = ESP_getFlashChipRealSize();
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

    Serial.printf("Reale Flash ID:   %08X\n", ESP_getFlashChipId());
    auto f = getCpuFrequencyMhz();
    Serial.printf("CPU Freq: %i\n", f);
}

#endif
