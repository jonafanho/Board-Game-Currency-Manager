#include <Adafruit_PN532.h>
#include "display.h"
#include "keypad.h"

#define AUDIO_PIN 18

#define CLK_PIN 13
#define MISO_PIN 12
#define MOSI_PIN 11
#define CS_NFC_PIN 17

#define LINE_HEIGHT 36

Adafruit_PN532 nfc(CLK_PIN, MISO_PIN, MOSI_PIN, CS_NFC_PIN);
Keypad keypad();

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

    gfx.begin();
    gfx.setRotation(1);
    gfx.fillScreen(TFT_BLACK);
    gfx.touch()->init();
    gfx.setBrightness(128);
}

int16_t touchX;
int16_t touchY;

void loop()
{
    uint8_t uid[7];    // Max 7 bytes
    uint8_t uidLength; // 4 or 7 bytes depending on tag type

    if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 50))
    {
        gfx.fillScreen(TFT_BLACK);
        gfx.setCursor(0, 0);
        gfx.setColor(TFT_WHITE);
        gfx.setFont(&fonts::FreeSans24pt7b);
        gfx.println("NFC Tag UID: ");

        for (uint8_t i = 0; i < uidLength; i++)
        {
            gfx.setCursor(0, (i + 1) * LINE_HEIGHT);
            gfx.println(uid[i], HEX);
        }

        ledcWriteTone(0, 740);
        ledcWrite(0, 0x7F);
        delay(200);
        ledcWrite(0, 0);
    }

    lgfx::touch_point_t tp;
    if (gfx.getTouch(&tp))
    {
        gfx.fillCircle(touchX, touchY, 10, TFT_BLACK);
        touchX = tp.x;
        touchY = tp.y;
        gfx.fillCircle(touchX, touchY, 10, TFT_SKYBLUE);
    }
}
