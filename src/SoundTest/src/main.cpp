#include <Arduino.h>
#include <DFPlayerMini.hpp>
#include <MemoryStream.hpp>

#if defined(ESP32)
#include "esp32.hpp"
#elif defined(ESP8266)
#include "esp8266.hpp"
#endif

#if !defined(LED_BUILTIN)
#define LED_BUILTIN 2
#endif

DFPlayerMini player;
    
void setup()
{
    Serial.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT);

#if(defined ESP32)
    PrintChipInfo();
    FPSerial.begin(9600, SERIAL_8N1, /*rx =*/RXD2, /*tx =*/TXD2);
#elif defined(ESP8266)
    FPSerial.begin(9600, SWSERIAL_8N1);
    FPSerial.listen();
#else
    MemoryStream FPSerial;
#endif
    if(!player.begin(&FPSerial)){ 
        Serial.println(F("Unable to connect player wait for reset"));
        while(true){
            delay(0); // Code to compatible with ESP8266 watch dog.
        }
    }

    player.volume(10);  //Set volume value. From 0 to 30
    player.firmwareVersion();
    player.playMp3Folder(1);
    
}

void loop()
{
    /*digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);*/
    if(player.available()) {
        uint16_t parameter;
        auto message = player.getMessage(parameter);
        if(message == PlayerMessage::PlayFinished) {
            player.next();
            auto fileNumber = player.readCurrentFileNumber(PlayerDevice::Card);
            Serial.printf("We play %d", fileNumber);
            Serial.println();
        }
    }
}
