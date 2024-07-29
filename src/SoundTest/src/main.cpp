#include <Arduino.h>
#include <DFPlayerMini.hpp>
#include <MemoryStream.hpp>
#include <SPI.h>
#include <TFT_eSPI.h>

#if defined(ESP32)
#include "esp32.hpp"
#elif defined(ESP8266)
#include "esp8266.hpp"
#endif

DFPlayerMini player;
TFT_eSPI tft = TFT_eSPI();

void setup()
{
    Serial.begin(115200);

#if defined(_DEBUG)
    delay(500); // Wait Terminal ist online
#endif

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

    tft.init();

    tft.setRotation(1);
    tft.setTextSize(2);
    tft.setCursor(0, 0);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE,TFT_BLACK);
    tft.println(F("Starting"));

    player.volume(10);  //Set volume value. From 0 to 30
    player.firmwareVersion();
    player.playMp3Folder(1);
    delay(200);
    
    auto state = player.getCurrentStatus();
    Serial.printf("player state %d", state);
    Serial.println();
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
            delay(200);
            auto fileNumber = player.readCurrentFileNumber(PlayerDevice::Card);
            Serial.printf("We play %d", fileNumber);
            Serial.println();
        }
    }
}
