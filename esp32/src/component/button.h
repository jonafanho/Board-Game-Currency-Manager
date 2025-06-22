#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

class Button
{
public:
	Button(
		const char *labelLeft, const char *labelCenter, const char *labelRight,
		const uint8_t *iconCenter, const uint8_t *iconRight,
		uint16_t x, uint16_t y, uint16_t width, uint16_t height,
		uint16_t backgroundColor, bool smallText, bool smallIcon);
	bool tick(int16_t rawTouchX, int16_t rawTouchY);

private:
	const char *labelLeft;
	const char *labelCenter;
	const char *labelRight;
	const uint8_t *iconCenter;
	const uint8_t *iconRight;
	const uint16_t x;
	const uint16_t y;
	const uint16_t width;
	const uint16_t height;
	const uint16_t backgroundColor;
	const bool smallText;
	const bool smallIcon;

	bool needsToDraw = true;
	bool previouslyTouching = false;
};

#endif
