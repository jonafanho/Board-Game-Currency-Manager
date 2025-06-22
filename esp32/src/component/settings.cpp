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
        Button menuPlayers = createMenuItem(PLAYERS_TEXT, playerCountText, menuY);

        char startingBalanceText[128];
        dtostrf((float)game->getStartingBalance() / DECIMAL_OFFSET_FACTOR, 1, game->getDecimalPlaces(), startingBalanceText);
        Button menuStartingBalance = createMenuItem(STARTING_BALANCE_TEXT, startingBalanceText, menuY += buttonHeight);

        char decimalPlacesText[4];
        sprintf(decimalPlacesText, "%u", game->getDecimalPlaces());
        Button menuDecimalPlaces = createMenuItem(DECIMAL_PLACES_TEXT, decimalPlacesText, menuY += buttonHeight);

        Button menuOverdraftHandling = createMenuItem(OVERDRAFT_HANDLING_TEXT, getOverdraftHandlingText(), menuY += buttonHeight);

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

        gfx.setSmallFont();
        for (uint8_t i = 0; i < game->getPlayerCount(); i++)
        {
            gfx.setCursor(PADDING, menuY + PADDING);
            gfx.print(i);

            char playerBalanceText[128];
            dtostrf((float)game->getPlayerBalance(i) / DECIMAL_OFFSET_FACTOR, 1, game->getDecimalPlaces(), playerBalanceText);

            gfx.setCursor(SCREEN_HEIGHT - PADDING - gfx.textWidth(playerBalanceText), menuY + PADDING);
            gfx.print(playerBalanceText);

            menuY += buttonHeight;
        }

        Button buttonAddPlayer = Button(nullptr, ADD_PLAYER_TEXT, nullptr, nullptr, nullptr, 0, SCREEN_WIDTH - buttonHeight * 2 - 1, SCREEN_HEIGHT / 2 - 1, buttonHeight, BUTTON_COLOR, true, false);
        Button buttonResetBalances = Button(nullptr, RESET_BALANCES_TEXT, nullptr, nullptr, nullptr, SCREEN_HEIGHT / 2, SCREEN_WIDTH - buttonHeight * 2 - 1, SCREEN_HEIGHT / 2, buttonHeight, BUTTON_COLOR, true, false);
        Button buttonOk = Button(nullptr, OK_TEXT, nullptr, nullptr, nullptr, 0, SCREEN_WIDTH - buttonHeight, SCREEN_HEIGHT, buttonHeight, BUTTON_COLOR, true, false);

        while (true)
        {
            lgfx::touch_point_t touchPoint;
            gfx.getTouch(&touchPoint);

            if (buttonAddPlayer.tick(touchPoint.x, touchPoint.y))
            {
                const uint8_t id[] = {0x12, 0x34};
                game->addPlayer(id, 2, TFT_DARKGRAY);
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

Button Settings::createMenuItem(const char *labelLeft, const char *labelRight, uint16_t y)
{
    return Button(labelLeft, nullptr, labelRight, nullptr, nullptr, 0, y, SCREEN_HEIGHT, buttonHeight, TFT_BLACK, true, true);
}

Button Settings::createMenuItemIcon(const char *labelLeft, const uint8_t *iconRight, uint16_t y)
{
    return Button(labelLeft, nullptr, nullptr, nullptr, iconRight, 0, y, SCREEN_HEIGHT, buttonHeight, TFT_BLACK, true, true);
}

Button Settings::createIconButton(const uint8_t *icon, uint16_t y)
{
    return Button(nullptr, nullptr, nullptr, icon, nullptr, SCREEN_HEIGHT - buttonHeight, y, buttonHeight, buttonHeight, TFT_BLACK, true, true);
}
