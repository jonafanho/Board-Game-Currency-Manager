#pragma once
#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480

class LGFX : public lgfx::LGFX_Device
{
    lgfx::Panel_RGB _panel;
    lgfx::Bus_RGB _bus;
    lgfx::Light_PWM _light;
    lgfx::Touch_GT911 _touch;

public:
    LGFX(void)
    {
        {
            auto cfg = _panel.config();

            cfg.memory_width = SCREEN_WIDTH;
            cfg.memory_height = SCREEN_HEIGHT;
            cfg.panel_width = SCREEN_WIDTH;
            cfg.panel_height = SCREEN_HEIGHT;

            _panel.config(cfg);
        }

        {
            auto cfg = _bus.config();

            cfg.pin_d0 = 15;
            cfg.pin_d1 = 7;
            cfg.pin_d2 = 6;
            cfg.pin_d3 = 5;
            cfg.pin_d4 = 4;
            cfg.pin_d5 = 9;
            cfg.pin_d6 = 46;
            cfg.pin_d7 = 3;
            cfg.pin_d8 = 8;
            cfg.pin_d9 = 16;
            cfg.pin_d10 = 1;
            cfg.pin_d11 = 14;
            cfg.pin_d12 = 21;
            cfg.pin_d13 = 47;
            cfg.pin_d14 = 48;
            cfg.pin_d15 = 45;

            cfg.pin_henable = 41;
            cfg.pin_vsync = 40;
            cfg.pin_hsync = 39;
            cfg.pin_pclk = 42;

            // From datasheet, doesn't actually work
            cfg.hsync_front_porch = 210;
            cfg.hsync_pulse_width = 30;
            cfg.hsync_back_porch = 16;
            cfg.vsync_front_porch = 22;
            cfg.vsync_pulse_width = 13;
            cfg.vsync_back_porch = 10;

            // What actually works
            cfg.hsync_front_porch = 8;
            cfg.hsync_pulse_width = 2;
            cfg.hsync_back_porch = 43;
            cfg.vsync_front_porch = 13;
            cfg.vsync_pulse_width = 2;
            cfg.vsync_back_porch = 12;

            cfg.freq_write = 12000000;

            cfg.panel = &_panel;

            _bus.config(cfg);
            _panel.setBus(&_bus);
        }

        {
            auto cfg = _light.config();

            cfg.pin_bl = 2;

            _light.config(cfg);
            _panel.setLight(&_light);
        }

        {
            auto cfg = _touch.config();

            cfg.i2c_port = 1;
            cfg.i2c_addr = 0x5D;
            cfg.pin_sda = 19;
            cfg.pin_scl = 20;

            cfg.x_min = 0;
            cfg.x_max = SCREEN_WIDTH;
            cfg.y_min = 0;
            cfg.y_max = SCREEN_HEIGHT;

            _touch.config(cfg);
            _panel.setTouch(&_touch);
        }

        setPanel(&_panel);
    }

    uint8_t setSmallFont()
    {
        setFont(&fonts::DejaVu18);
        return 18;
    }

    uint8_t setMediumFont()
    {
        setFont(&fonts::DejaVu40);
        return 40;
    }

    uint8_t setLargeFont()
    {
        setFont(&fonts::DejaVu72);
        return 72;
    }
};

class LGFX_Sprite_New : public lgfx::LGFX_Sprite
{
public:
    LGFX_Sprite_New(LGFX *parent) : lgfx::LGFX_Sprite(parent) {}
};

extern LGFX gfx;
extern LGFX_Sprite_New sprite;
