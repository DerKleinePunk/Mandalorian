#pragma once

#include <lvgl.h>

class Gui
{
private:
    TFT_eSPI* _tft;
    ButtonWidget* _btnOne;
    ButtonWidget* _btnTwo;
public:
    Gui(TFT_eSPI* tft);
    ~Gui();

    void begin();
    void update(bool pressed, const uint16_t& x, const uint16_t& y);
};
