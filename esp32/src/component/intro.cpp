#include "intro.h"
#include "button.h"
#include "constants.h"

void Intro::show()
{
    gfx.fillScreen(TFT_BLACK);

    uint8_t textHeight = gfx.setLargeFont();
    gfx.setCursor(SCREEN_HEIGHT / 2 - gfx.textWidth(WELCOME_TEXT_1) / 2, SCREEN_WIDTH / 2 - textHeight / 2);
    gfx.setTextColor(TFT_WHITE);
    gfx.print(WELCOME_TEXT_1);

    textHeight = gfx.setSmallFont();
    gfx.setCursor(SCREEN_HEIGHT / 2 - gfx.textWidth(WELCOME_TEXT_2) / 2, SCREEN_WIDTH - textHeight);
    gfx.setTextColor(TFT_DARKGRAY);
    gfx.print(WELCOME_TEXT_2);

    bool previouslyTouched;

    while (true)
    {
        lgfx::touch_point_t touchPoint;
        bool touched = gfx.getTouch(&touchPoint);
        if (previouslyTouched && !touched)
        {
            return;
        }
        previouslyTouched = touched;
        delay(50);
    }
}
