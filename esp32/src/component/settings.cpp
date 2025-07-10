#include <vector>
#include "hardware/display.h"
#include "hardware/nfc.h"
#include "settings.h"
#include "button.h"
#include "keypad.h"
#include "constants.h"
#include "icons.h"

Settings::Settings(Game *game) : game(game)
{
    buttonHeight = gfx.setSmallFont() + PADDING * 2;
}

void Settings::show()
{
    while (true)
    {
        gfx.fillScreen(TFT_BLACK);

        uint16_t menuY = gfx.setMediumFont() + PADDING * 2;
        gfx.setCursor(PADDING, PADDING);
        gfx.setTextColor(TFT_WHITE);
        gfx.print(SETTINGS_TEXT);

        char playerCountText[4];
        sprintf(playerCountText, "%u", game->getPlayerCount());
        Button menuPlayers = createMenuItem(PLAYERS_TEXT, playerCountText, 0, menuY);

        char startingBalanceText[128];
        dtostrf((float)game->getStartingBalance() / DECIMAL_OFFSET_FACTOR, 1, game->getDecimalPlaces(), startingBalanceText);
        Button menuStartingBalance = createMenuItem(STARTING_BALANCE_TEXT, startingBalanceText, 0, menuY += buttonHeight);

        char decimalPlacesText[4];
        sprintf(decimalPlacesText, "%u", game->getDecimalPlaces());
        Button menuDecimalPlaces = createMenuItem(DECIMAL_PLACES_TEXT, decimalPlacesText, 0, menuY += buttonHeight);

        Button menuOverdraftHandling = createMenuItem(OVERDRAFT_HANDLING_TEXT, getOverdraftHandlingText(), 0, menuY += buttonHeight);

        Button buttonOk = Button(nullptr, OK_TEXT, nullptr, nullptr, nullptr, 0, SCREEN_WIDTH - buttonHeight, SCREEN_HEIGHT / 2 - 1, buttonHeight, BUTTON_COLOR, true, false);
        Button buttonReset = Button(nullptr, RESET_ALL_TEXT, nullptr, nullptr, nullptr, SCREEN_HEIGHT / 2, SCREEN_WIDTH - buttonHeight, SCREEN_HEIGHT / 2, buttonHeight, TFT_RED, true, false);

        while (true)
        {
            lgfx::touch_point_t touchPoint;
            gfx.getTouch(&touchPoint);

            if (menuPlayers.tick(touchPoint.x, touchPoint.y))
            {
                showPlayersMenu();
                break;
            }

            if (menuStartingBalance.tick(touchPoint.x, touchPoint.y))
            {
                gfx.fillScreen(TFT_BLACK);
                gfx.setMediumFont();
                gfx.setCursor(PADDING, PADDING);
                gfx.setTextColor(TFT_WHITE);
                gfx.print(STARTING_BALANCE_TEXT);

                int32_t startingBalance = Keypad(6, game->getDecimalPlaces()).show();
                if (startingBalance >= 0)
                {
                    game->setStartingBalance(startingBalance);
                }
                break;
            }

            if (menuDecimalPlaces.tick(touchPoint.x, touchPoint.y))
            {
                uint8_t decimalPlaces = showDecimalPlacesMenu();
                if (decimalPlaces <= 2)
                {
                    game->setDecimalPlaces(decimalPlaces);
                }
                break;
            }

            if (menuOverdraftHandling.tick(touchPoint.x, touchPoint.y))
            {
                OverdraftHandling overdraftHandling = showOverdraftHandlingMenu();
                game->setOverdraftHandling(overdraftHandling);
                break;
            }

            if (buttonOk.tick(touchPoint.x, touchPoint.y))
            {
                return;
            }

            if (buttonReset.tick(touchPoint.x, touchPoint.y))
            {
                if (showResetConfirmation())
                {
                    game->reset();
                    return;
                }
                else
                {
                    break;
                }
            }
        }
    }
}

void Settings::showPlayersMenu()
{
    while (true)
    {
        gfx.fillScreen(TFT_BLACK);

        uint16_t menuY = gfx.setMediumFont() + PADDING * 2;
        gfx.setCursor(PADDING, PADDING);
        gfx.setTextColor(TFT_WHITE);
        gfx.print(PLAYERS_TEXT);

        std::vector<Button> buttons;
        char playerNames[MAX_PLAYERS][16];
        char playerBalances[MAX_PLAYERS][128];

        for (uint8_t i = 0; i < MAX_PLAYERS; i++)
        {
            dtostrf((float)game->getPlayerBalance(i) / DECIMAL_OFFSET_FACTOR, 1, game->getDecimalPlaces(), playerBalances[i]);
            sprintf(playerNames[i], "Player %d", i + 1);
            buttons.push_back(createMenuItem(playerNames[i], playerBalances[i], PADDING / 2, menuY));
            gfx.fillRect(0, menuY, PADDING / 2, buttonHeight, game->getPlayerColor(i));
            menuY += buttonHeight;
        }

        Button buttonAddPlayer = Button(nullptr, ADD_PLAYER_TEXT, nullptr, nullptr, nullptr, 0, SCREEN_WIDTH - buttonHeight * 2 - 1, SCREEN_HEIGHT / 2 - 1, buttonHeight, BUTTON_COLOR, true, false);
        Button buttonResetBalances = Button(nullptr, RESET_BALANCES_TEXT, nullptr, nullptr, nullptr, SCREEN_HEIGHT / 2, SCREEN_WIDTH - buttonHeight * 2 - 1, SCREEN_HEIGHT / 2, buttonHeight, BUTTON_COLOR, true, false);
        Button buttonOk = Button(nullptr, OK_TEXT, nullptr, nullptr, nullptr, 0, SCREEN_WIDTH - buttonHeight, SCREEN_HEIGHT, buttonHeight, BUTTON_COLOR, true, false);

        uint8_t playerCount = game->getPlayerCount();

        while (true)
        {
            lgfx::touch_point_t touchPoint;
            gfx.getTouch(&touchPoint);

            bool playerButtonPressed = false;
            for (uint8_t i = 0; i < playerCount; i++)
            {
                if (buttons[i].tick(touchPoint.x, touchPoint.y))
                {
                    game->setPlayerColor(i, showPlayerColorMenu(game->getPlayerColor(i)));
                    playerButtonPressed = true;
                    break;
                }
            }
            if (playerButtonPressed)
            {
                break;
            }

            if (buttonAddPlayer.tick(touchPoint.x, touchPoint.y))
            {
                showAddPlayerMenu();
                break;
            }

            if (buttonResetBalances.tick(touchPoint.x, touchPoint.y))
            {
                if (showResetBalancesConfirmation())
                {
                    game->resetPlayerBalances();
                }
                break;
            }

            if (buttonOk.tick(touchPoint.x, touchPoint.y))
            {
                return;
            }
        }
    }
}

void Settings::showAddPlayerMenu()
{
    uint16_t color = TFT_DARKGRAY;

    while (true)
    {
        gfx.fillScreen(TFT_BLACK);
        gfx.drawBitmap(SCREEN_HEIGHT / 2 - ICON_SIZE_LARGE / 2, SCREEN_WIDTH / 2 - ICON_SIZE_LARGE, iconLargeScan, ICON_SIZE_LARGE, ICON_SIZE_LARGE, color);

        uint8_t textHeightMedium = gfx.setMediumFont();
        gfx.setCursor(SCREEN_HEIGHT / 2 - gfx.textWidth(ADD_PLAYER_TEXT) / 2, SCREEN_WIDTH / 2);
        gfx.setTextColor(TFT_WHITE);
        gfx.print(ADD_PLAYER_TEXT);

        gfx.setSmallFont();
        gfx.setCursor(SCREEN_HEIGHT / 2 - gfx.textWidth(ADD_PLAYER_TEXT_1) / 2, SCREEN_WIDTH / 2 + textHeightMedium);
        gfx.setTextColor(TFT_DARKGRAY);
        gfx.print(ADD_PLAYER_TEXT_1);

        Button buttonSetPlayerColor = Button(nullptr, PLAYER_COLOR_TEXT, nullptr, nullptr, nullptr, 0, SCREEN_WIDTH - buttonHeight, SCREEN_HEIGHT / 2 - 1, buttonHeight, BUTTON_COLOR, true, false);
        Button buttonCancel = Button(nullptr, CANCEL_TEXT, nullptr, nullptr, nullptr, SCREEN_HEIGHT / 2, SCREEN_WIDTH - buttonHeight, SCREEN_HEIGHT / 2, buttonHeight, BUTTON_COLOR, true, false);

        while (true)
        {
            uint8_t nfcId[7]; // Max 7 bytes
            uint8_t length;   // 4 or 7 bytes depending on tag type
            if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, nfcId, &length, 50))
            {
                delay(50);
                game->addPlayer(nfcId, length, color);
                ledcWriteTone(0, 740);
                ledcWrite(0, 0x7F);
                delay(200);
                ledcWrite(0, 0);
                delay(50);
                return;
            }

            lgfx::touch_point_t touchPoint;
            gfx.getTouch(&touchPoint);

            if (buttonSetPlayerColor.tick(touchPoint.x, touchPoint.y))
            {
                color = showPlayerColorMenu(color);
                break;
            }

            if (buttonCancel.tick(touchPoint.x, touchPoint.y))
            {
                return;
            }
        }
    }
}

bool Settings::showResetBalancesConfirmation()
{
    gfx.fillScreen(TFT_BLACK);
    gfx.drawBitmap(SCREEN_HEIGHT / 2 - ICON_SIZE_LARGE / 2, SCREEN_WIDTH / 2 - ICON_SIZE_LARGE, iconLargeInfo, ICON_SIZE_LARGE, ICON_SIZE_LARGE, TFT_SKYBLUE);

    uint8_t textHeightMedium = gfx.setMediumFont();
    gfx.setCursor(SCREEN_HEIGHT / 2 - gfx.textWidth(RESET_BALANCES_TEXT_1) / 2, SCREEN_WIDTH / 2);
    gfx.setTextColor(TFT_WHITE);
    gfx.print(RESET_BALANCES_TEXT_1);

    uint8_t textHeightSmall = gfx.setSmallFont();
    gfx.setCursor(SCREEN_HEIGHT / 2 - gfx.textWidth(RESET_BALANCES_TEXT_2) / 2, SCREEN_WIDTH / 2 + textHeightMedium);
    gfx.setTextColor(TFT_DARKGRAY);
    gfx.print(RESET_BALANCES_TEXT_2);

    gfx.setCursor(SCREEN_HEIGHT / 2 - gfx.textWidth(RESET_BALANCES_TEXT_3) / 2, SCREEN_WIDTH / 2 + textHeightMedium + textHeightSmall);
    gfx.setTextColor(TFT_DARKGRAY);
    gfx.print(RESET_BALANCES_TEXT_3);

    Button buttonResetBalances = Button(nullptr, RESET_BALANCES_TEXT, nullptr, nullptr, nullptr, 0, SCREEN_WIDTH - buttonHeight, SCREEN_HEIGHT / 2 - 1, buttonHeight, BUTTON_COLOR, true, false);
    Button buttonCancel = Button(nullptr, CANCEL_TEXT, nullptr, nullptr, nullptr, SCREEN_HEIGHT / 2, SCREEN_WIDTH - buttonHeight, SCREEN_HEIGHT / 2, buttonHeight, BUTTON_COLOR, true, false);

    while (true)
    {
        lgfx::touch_point_t touchPoint;
        gfx.getTouch(&touchPoint);

        if (buttonResetBalances.tick(touchPoint.x, touchPoint.y))
        {
            return true;
        }

        if (buttonCancel.tick(touchPoint.x, touchPoint.y))
        {
            return false;
        }
    }
}

uint8_t Settings::showDecimalPlacesMenu()
{
    gfx.fillScreen(TFT_BLACK);

    uint16_t menuY = gfx.setMediumFont() + PADDING * 2;
    gfx.setCursor(PADDING, PADDING);
    gfx.setTextColor(TFT_WHITE);
    gfx.print(DECIMAL_PLACES_TEXT);

    uint16_t buttonSize = SCREEN_HEIGHT / 3;
    uint16_t buttonSizePadded = buttonSize - KEYPAD_PADDING * 2;
    Button button0 = Button(nullptr, KEYPAD_0_TEXT, nullptr, nullptr, nullptr, KEYPAD_PADDING, menuY, buttonSizePadded, buttonSizePadded, BUTTON_COLOR, false, false);
    Button button1 = Button(nullptr, KEYPAD_1_TEXT, nullptr, nullptr, nullptr, KEYPAD_PADDING + buttonSize, menuY, buttonSizePadded, buttonSizePadded, BUTTON_COLOR, false, false);
    Button button2 = Button(nullptr, KEYPAD_2_TEXT, nullptr, nullptr, nullptr, KEYPAD_PADDING + buttonSize * 2, menuY, buttonSizePadded, buttonSizePadded, BUTTON_COLOR, false, false);
    Button buttonCancel = Button(nullptr, CANCEL_TEXT, nullptr, nullptr, nullptr, 0, SCREEN_WIDTH - buttonHeight, SCREEN_HEIGHT, buttonHeight, BUTTON_COLOR, true, false);

    while (true)
    {
        lgfx::touch_point_t touchPoint;
        gfx.getTouch(&touchPoint);

        if (button0.tick(touchPoint.x, touchPoint.y))
        {
            return 0;
        }

        if (button1.tick(touchPoint.x, touchPoint.y))
        {
            return 1;
        }

        if (button2.tick(touchPoint.x, touchPoint.y))
        {
            return 2;
        }

        if (buttonCancel.tick(touchPoint.x, touchPoint.y))
        {
            return 3;
        }
    }
}

OverdraftHandling Settings::showOverdraftHandlingMenu()
{
    OverdraftHandling existingOverdraftHandling = game->getOverdraftHandling();
    gfx.fillScreen(TFT_BLACK);

    uint16_t menuY = gfx.setMediumFont() + PADDING * 2;
    gfx.setCursor(PADDING, PADDING);
    gfx.setTextColor(TFT_WHITE);
    gfx.print(OVERDRAFT_HANDLING_TEXT);

    Button buttonOverdraftHandling1 = createMenuItemIcon(OVERDRAFT_HANDLING_1_TEXT, existingOverdraftHandling == OverdraftHandling::BlockTransaction ? iconSmallCheck : nullptr, menuY);
    Button buttonOverdraftHandling2 = createMenuItemIcon(OVERDRAFT_HANDLING_2_TEXT, existingOverdraftHandling == OverdraftHandling::ClampToZero ? iconSmallCheck : nullptr, menuY += buttonHeight);
    Button buttonOverdraftHandling3 = createMenuItemIcon(OVERDRAFT_HANDLING_3_TEXT, existingOverdraftHandling == OverdraftHandling::Allow ? iconSmallCheck : nullptr, menuY += buttonHeight);
    Button buttonCancel = Button(nullptr, CANCEL_TEXT, nullptr, nullptr, nullptr, 0, SCREEN_WIDTH - buttonHeight, SCREEN_HEIGHT, buttonHeight, BUTTON_COLOR, true, false);

    while (true)
    {
        lgfx::touch_point_t touchPoint;
        gfx.getTouch(&touchPoint);

        if (buttonOverdraftHandling1.tick(touchPoint.x, touchPoint.y))
        {
            return OverdraftHandling::BlockTransaction;
        }

        if (buttonOverdraftHandling2.tick(touchPoint.x, touchPoint.y))
        {
            return OverdraftHandling::ClampToZero;
        }

        if (buttonOverdraftHandling3.tick(touchPoint.x, touchPoint.y))
        {
            return OverdraftHandling::Allow;
        }

        if (buttonCancel.tick(touchPoint.x, touchPoint.y))
        {
            return existingOverdraftHandling;
        }
    }
}

uint16_t Settings::showPlayerColorMenu(uint16_t existingColor)
{
    gfx.fillScreen(TFT_BLACK);

    uint16_t menuY = gfx.setMediumFont() + PADDING * 2;
    gfx.setCursor(PADDING, PADDING);
    gfx.setTextColor(TFT_WHITE);
    gfx.print(PLAYER_COLOR_TEXT);

    uint16_t buttonSize = SCREEN_HEIGHT / 4;
    uint16_t buttonSizePadded = buttonSize - KEYPAD_PADDING * 2;
    std::vector<Button> buttons;
    const uint16_t playerColors[] = {TFT_RED, TFT_MAROON, TFT_BROWN, TFT_ORANGE, TFT_YELLOW, TFT_GREENYELLOW, TFT_GREEN, TFT_DARKGREEN, TFT_CYAN, TFT_SKYBLUE, TFT_BLUE, TFT_PURPLE, TFT_MAGENTA, TFT_PINK, TFT_WHITE, TFT_DARKGRAY};
    const size_t playerColorsCount = sizeof(playerColors) / sizeof(playerColors[0]);

    for (uint8_t i = 0; i < playerColorsCount; i++)
    {
        uint16_t color = playerColors[i];
        buttons.push_back(createIconButton(color == existingColor ? iconCheck : nullptr, (i % 4) * buttonSize + KEYPAD_PADDING, menuY, buttonSizePadded, buttonSizePadded, color));
        if (i % 4 == 3)
        {
            menuY += buttonSize;
        }
    }

    Button buttonCancel = Button(nullptr, CANCEL_TEXT, nullptr, nullptr, nullptr, 0, SCREEN_WIDTH - buttonHeight, SCREEN_HEIGHT, buttonHeight, BUTTON_COLOR, true, false);

    while (true)
    {
        lgfx::touch_point_t touchPoint;
        gfx.getTouch(&touchPoint);

        for (size_t i = 0; i < playerColorsCount; i++)
        {
            if (buttons[i].tick(touchPoint.x, touchPoint.y))
            {
                return playerColors[i];
            }
        }

        if (buttonCancel.tick(touchPoint.x, touchPoint.y))
        {
            return existingColor;
        }
    }
}

bool Settings::showResetConfirmation()
{
    gfx.fillScreen(TFT_BLACK);
    gfx.drawBitmap(SCREEN_HEIGHT / 2 - ICON_SIZE_LARGE / 2, SCREEN_WIDTH / 2 - ICON_SIZE_LARGE, iconLargeWarning, ICON_SIZE_LARGE, ICON_SIZE_LARGE, TFT_YELLOW);

    uint8_t textHeight = gfx.setMediumFont();
    gfx.setCursor(SCREEN_HEIGHT / 2 - gfx.textWidth(RESET_TEXT_1) / 2, SCREEN_WIDTH / 2);
    gfx.setTextColor(TFT_WHITE);
    gfx.print(RESET_TEXT_1);

    gfx.setSmallFont();
    gfx.setCursor(SCREEN_HEIGHT / 2 - gfx.textWidth(RESET_TEXT_2) / 2, SCREEN_WIDTH / 2 + textHeight);
    gfx.setTextColor(TFT_DARKGRAY);
    gfx.print(RESET_TEXT_2);

    Button buttonReset = Button(nullptr, RESET_ALL_TEXT, nullptr, nullptr, nullptr, 0, SCREEN_WIDTH - buttonHeight, SCREEN_HEIGHT / 2 - 1, buttonHeight, TFT_RED, true, false);
    Button buttonCancel = Button(nullptr, CANCEL_TEXT, nullptr, nullptr, nullptr, SCREEN_HEIGHT / 2, SCREEN_WIDTH - buttonHeight, SCREEN_HEIGHT / 2, buttonHeight, BUTTON_COLOR, true, false);

    while (true)
    {
        lgfx::touch_point_t touchPoint;
        gfx.getTouch(&touchPoint);

        if (buttonReset.tick(touchPoint.x, touchPoint.y))
        {
            return true;
        }

        if (buttonCancel.tick(touchPoint.x, touchPoint.y))
        {
            return false;
        }
    }
}

const char *Settings::getOverdraftHandlingText()
{
    OverdraftHandling overdraftHandling = game->getOverdraftHandling();
    switch (overdraftHandling)
    {
    case OverdraftHandling::BlockTransaction:
        return OVERDRAFT_HANDLING_1_TEXT;
    case OverdraftHandling::ClampToZero:
        return OVERDRAFT_HANDLING_2_TEXT;
    default:
        return OVERDRAFT_HANDLING_3_TEXT;
    }
}

Button Settings::createMenuItem(const char *labelLeft, const char *labelRight, uint16_t x, uint16_t y)
{
    return Button(labelLeft, nullptr, labelRight, nullptr, nullptr, x, y, SCREEN_HEIGHT - x, buttonHeight, TFT_BLACK, true, true);
}

Button Settings::createMenuItemIcon(const char *labelLeft, const uint8_t *iconRight, uint16_t y)
{
    return Button(labelLeft, nullptr, nullptr, nullptr, iconRight, 0, y, SCREEN_HEIGHT, buttonHeight, TFT_BLACK, true, true);
}

Button Settings::createIconButton(const uint8_t *icon, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
{
    return Button(nullptr, nullptr, nullptr, icon, nullptr, x, y, width, height, color, true, false);
}
