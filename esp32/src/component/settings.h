#ifndef SETTINGS_H
#define SETTINGS_H

#include "data/game.h"
#include "hardware/display.h"

class Button;

class Settings
{
public:
    Settings(Game *game);
    void show();

private:
    Game *game;
    uint8_t buttonHeight;

    void showPlayersMenu();
    bool showResetBalancesConfirmation();
    uint8_t showDecimalPlacesMenu();
    OverdraftHandling showOverdraftHandlingMenu();
    bool showResetConfirmation();
    const char *getOverdraftHandlingText();
    Button createMenuItem(const char *labelLeft, const char *labelRight, uint16_t y);
    Button createMenuItemIcon(const char *labelLeft, const uint8_t *iconRight, uint16_t y);
    Button createIconButton(const uint8_t *icon, uint16_t y);
};

#endif
