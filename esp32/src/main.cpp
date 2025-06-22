#include <Adafruit_PN532.h>
#include <LittleFS.h>
#include "hardware/display.h"
#include "data/game.h"
#include "component/intro.h"
#include "component/settings.h"
#include "component/keypad.h"

#define AUDIO_PIN 18

#define CLK_PIN 13
#define MISO_PIN 12
#define MOSI_PIN 11
#define CS_NFC_PIN 17

Adafruit_PN532 nfc(CLK_PIN, MISO_PIN, MOSI_PIN, CS_NFC_PIN);

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

    gfx.begin();
    gfx.setRotation(1);
    gfx.fillScreen(TFT_BLACK);
    gfx.setTextWrap(false);
    gfx.touch()->init();
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

    uint8_t uid[7];    // Max 7 bytes
    uint8_t uidLength; // 4 or 7 bytes depending on tag type

    if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 50))
    {
        gfx.fillScreen(TFT_BLACK);
        gfx.println("NFC Tag UID: ");

        for (uint8_t i = 0; i < uidLength; i++)
        {
            gfx.setCursor(0, (i + 1) * 36);
            gfx.println(uid[i], HEX);
        }

        ledcWriteTone(0, 740);
        ledcWrite(0, 0x7F);
        delay(200);
        ledcWrite(0, 0);
    }
}
