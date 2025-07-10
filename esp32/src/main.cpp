#include <LittleFS.h>
#include "hardware/display.h"
#include "hardware/nfc.h"
#include "data/game.h"
#include "component/intro.h"
#include "component/settings.h"
#include "component/keypad.h"

void setup()
{
    gfx.setBrightness(0);
    Serial.begin(9600);
    Serial.println("");

    nfc.begin();
    while (true)
    {
        uint32_t versiondata = nfc.getFirmwareVersion();
        if (versiondata != 0)
        {
            Serial.print("Found chip PN5");
            Serial.println((versiondata >> 24) & 0xFF, HEX);
            Serial.print("Firmware version: ");
            Serial.print((versiondata >> 16) & 0xFF, DEC);
            Serial.print('.');
            Serial.println((versiondata >> 8) & 0xFF, DEC);
            break;
        }
    }
    nfc.SAMConfig();

    ledcSetup(0, 740, 8);
    ledcAttachPin(AUDIO_PIN, 0);

    Serial.println(LittleFS.begin() ? "File system mounted successfully" : "File system failed to mount!");

    gfx.init();
    gfx.setRotation(1);
    gfx.fillScreen(TFT_BLACK);
    gfx.setTextWrap(false);
    gfx.touch()->init();
    sprite.setColorDepth(gfx.getColorDepth());
    sprite.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT);
    gfx.setBrightness(128);
}

void loop()
{
    Game game = Game::readFromMemory();

    if (game.getPlayerCount() == 0 || true)
    {
        Intro().show();
        Settings(&game).show();
    }
}
