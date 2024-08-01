#pragma once

#include <TFT_eSPI.h>
#include <lvgl.h>

class Gui
{
private:
    lv_display_t * _display;
    TFT_eSPI* _tft;
public:
    Gui(TFT_eSPI* tft);
    ~Gui();

    void begin();
    void update(bool pressed, const uint16_t& x, const uint16_t& y);
};
