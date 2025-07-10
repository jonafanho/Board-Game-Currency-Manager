#ifndef SETTINGS_H
#define SETTINGS_H

#include "data/game.h"

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
    void showAddPlayerMenu();
    bool showResetBalancesConfirmation();
    uint8_t showDecimalPlacesMenu();
    OverdraftHandling showOverdraftHandlingMenu();
    uint16_t showPlayerColorMenu(uint16_t existingColor);
    bool showResetConfirmation();
    const char *getOverdraftHandlingText();
    Button createMenuItem(const char *labelLeft, const char *labelRight, uint16_t x, uint16_t y);
    Button createMenuItemIcon(const char *labelLeft, const uint8_t *iconRight, uint16_t y);
    Button createIconButton(const uint8_t *icon, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
};

#endif
