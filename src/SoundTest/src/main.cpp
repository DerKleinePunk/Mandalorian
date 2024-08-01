#include <Arduino.h>
#include <Preferences.h>
#include <SPI.h>
#include <TFT_eSPI.h>

#include <DFPlayerMini.hpp>
#include <MemoryStream.hpp>

#include "Gui.hpp"

#if defined(ESP32)
#include "esp32.hpp"
#elif defined(ESP8266)
#include "esp8266.hpp"
#endif

DFPlayerMini player;
TFT_eSPI tft = TFT_eSPI();
Preferences preferences;
Gui ui(&tft);

#define REPEAT_CALIBRATE false

void touch_calibrate()
{
    uint16_t calData[5];
    auto exists = preferences.isKey("TouchCali");

    if(!exists || REPEAT_CALIBRATE) {
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(20, 0);
        tft.setTextFont(2);
        tft.setTextSize(1);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);

        tft.println(F("Touch corners as indicated"));

        tft.setTextFont(1);
        tft.println();

        if (REPEAT_CALIBRATE) {
            tft.setTextColor(TFT_RED, TFT_BLACK);
            tft.println(F("Set REPEAT_CAL to false to stop this running again!"));
        }

        tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.println(F("Calibration complete!"));
        Serial.printf("Cali %d %d %d %d %d\n", calData[0], calData[1], calData[2], calData[3], calData[4]);
        preferences.putBytes("TouchCali", calData, 10);

    } else {
        preferences.getBytes("TouchCali", calData, 10);
        Serial.printf("Cali %d %d %d %d %d\n", calData[0], calData[1], calData[2], calData[3], calData[4]);
        tft.setTouch(calData);
    }
}

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
    if(!player.begin(&FPSerial)) {
        Serial.println(F("Unable to connect player wait for reset"));
        while(true) {
            delay(0); // Code to compatible with ESP8266 watch dog.
        }
    }

    tft.init();

    tft.setRotation(0);
    tft.setTextSize(2);
    tft.setCursor(0, 0);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.println(F("Starting..."));

    preferences.begin("Mando", false);

    player.volume(10); // Set volume value. From 0 to 30
    player.firmwareVersion();
    player.playMp3Folder(1);
    delay(200);

    auto state = player.getCurrentStatus();
    Serial.printf("player state %d", state);
    Serial.println();

    touch_calibrate();
    ui.begin();
}

void loop()
{
    static uint32_t scanTime = millis();

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

    lv_task_handler();

    uint16_t t_x = 0, t_y = 0;
    if (millis() - scanTime >= 50) {
        bool pressed = tft.getTouch(&t_x, &t_y);
        scanTime = millis();
        ui.update(pressed, t_x, t_y);

        if(pressed) {
            Serial.println("X= " + String(t_x) + " | Y= " + String(t_y));
        }

    }
}
