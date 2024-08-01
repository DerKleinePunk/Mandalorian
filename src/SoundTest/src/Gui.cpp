#include "Gui.hpp"

static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];

#if USE_LV_LOG != 0
/* Serial debugging */
void my_print(lv_log_level_t level, const char * file, uint32_t line, const char * dsc)
{

    Serial.printf("%s@%d->%s\r\n", file, line, dsc);
    Serial.flush();
}
#endif

Gui::Gui(TFT_eSPI* tft)
{
    _tft = tft;
    _btnOne = new ButtonWidget(tft);
    _btnTwo = new ButtonWidget(tft);
}

Gui::~Gui()
{
}

void Gui::begin()
{
    uint16_t x = (_tft->width() - BUTTON_W) / 2;
    uint16_t y = _tft->height() / 2 - BUTTON_H - 10;
    _btnOne->initButtonUL(x, y, BUTTON_W, BUTTON_H, TFT_WHITE, TFT_RED, TFT_BLACK, "Button", 1);
    // btnOne->setPressAction(btnL_pressAction);
    // btnOne->setReleaseAction(btnL_releaseAction);
    _btnOne->drawSmoothButton(false, 3, TFT_BLACK); // 3 is outline width, TFT_BLACK is the surrounding background colour for anti-aliasing

    y = _tft->height() / 2 + 10;
    _btnTwo->initButtonUL(x, y, BUTTON_W, BUTTON_H, TFT_WHITE, TFT_BLACK, TFT_GREEN, "Off", 1);
    // btnTwo->setPressAction(btnR_pressAction);
    // btnTwo->.setReleaseAction(btnR_releaseAction);
    _btnTwo->drawSmoothButton(false, 3, TFT_BLACK); // 3 is outline width, TFT_BLACK is the surrounding background colour for anti-aliasing
}

void Gui::update(bool pressed, const uint16_t& x, const uint16_t& y)
{
    // for (uint8_t b = 0; b < buttonCount; b++) {
    if(pressed) {
        if(_btnOne->contains(x, y)) {
            _btnOne->press(true);
            _btnOne->pressAction();
        }
    } else {
        if(_btnOne->isPressed()) {
            _btnOne->press(false);
            _btnOne->releaseAction();
        }
    }
}
