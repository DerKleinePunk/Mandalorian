#include "Gui.hpp"
#include <lvgl.h>
#include <esp_log.h>

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HEIGHT * TFT_WIDTH / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];
TFT_eSPI* g_tft;

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(lv_log_level_t level, const char * buf)
{
    LV_UNUSED(level);
    Serial.printf("%d->%s\r\n", level, buf);
    Serial.flush();
}
#endif

static uint32_t my_tick(void)
{
    return millis();
}

/*Read the touchpad*/
void my_touchpad_read( lv_indev_t * indev, lv_indev_data_t * data )
{
    uint16_t t_x = 0, t_y = 0;

    bool touched = g_tft->getTouch(&t_x, &t_y);

    if(!touched) {
        data->state = LV_INDEV_STATE_RELEASED;
    } else {
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = t_x;
        data->point.y = t_y;
    }
}

static void btn_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = reinterpret_cast<lv_obj_t *>(lv_event_get_target(e));
    if(code == LV_EVENT_CLICKED) {
        static uint8_t cnt = 0;
        cnt++;

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}


Gui::Gui(TFT_eSPI* tft)
{
    _tft = tft;
    //Todo Make it right
    g_tft = tft;
}

Gui::~Gui()
{
}

void Gui::begin()
{
    lv_init();

#if LV_USE_LOG != 0
    lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

    lv_tick_set_cb(my_tick);
    _display = lv_tft_espi_create(TFT_WIDTH, TFT_HEIGHT, draw_buf, sizeof(draw_buf));
    lv_display_set_rotation(_display, LV_DISPLAY_ROTATION_90);

    lv_indev_t * indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); /*Touchpad should have POINTER type*/
    lv_indev_set_read_cb(indev, my_touchpad_read);

    lv_obj_t *label = lv_label_create( lv_scr_act() );
    lv_label_set_text( label, "Hello Arduino, I'm LVGL!" );
    lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );

    lv_obj_t * btn = lv_button_create(lv_screen_active());     /*Add a button the current screen*/
    lv_obj_set_pos(btn, 10, 10);                            /*Set its position*/
    lv_obj_set_size(btn, 120, 50);                          /*Set its size*/
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);           /*Assign a callback to the button*/

    lv_obj_t * label2 = lv_label_create(btn);          /*Add a label to the button*/
    lv_label_set_text(label2, "Button");                     /*Set the labels text*/
    lv_obj_center(label2);

    Serial.println( "Ui Setup done" );
}

void Gui::update(bool pressed, const uint16_t& x, const uint16_t& y)
{

}
