#include "button.h"
#include "hardware/display.h"
#include "constants.h"
#include "icons.h"

Button::Button(
	const char *labelLeft, const char *labelCenter, const char *labelRight,
	const uint8_t *iconCenter, const uint8_t *iconRight,
	uint16_t x, uint16_t y, uint16_t width, uint16_t height,
	uint16_t backgroundColor, bool smallText, bool smallIcon)
	: labelLeft(labelLeft), labelCenter(labelCenter), labelRight(labelRight),
	  iconCenter(iconCenter),
	  iconRight(iconRight),
	  x(x), y(y), width(width), height(height),
	  backgroundColor(backgroundColor), smallText(smallText), smallIcon(smallIcon) {}

bool Button::tick(int16_t rawTouchX, int16_t rawTouchY)
{
	bool touchingButton = rawTouchX >= x && rawTouchX < x + width && rawTouchY >= y && rawTouchY < y + height;

	if (touchingButton != previouslyTouching || needsToDraw)
	{
		gfx.fillRect(x, y, width, height, touchingButton ? TFT_DARKGRAY : backgroundColor);

		if (labelLeft != nullptr)
		{
			uint8_t textHeight = smallText ? gfx.setSmallFont() : gfx.setMediumFont();
			gfx.setTextColor(TFT_WHITE, -1);
			gfx.setCursor(x + PADDING, y + height / 2 - textHeight / 2);
			gfx.print(labelLeft);
		}

		if (labelCenter != nullptr)
		{
			uint8_t textHeight = smallText ? gfx.setSmallFont() : gfx.setMediumFont();
			gfx.setTextColor(TFT_WHITE, -1);
			gfx.setCursor(x + width / 2 - gfx.textWidth(labelCenter) / 2, y + height / 2 - textHeight / 2);
			gfx.print(labelCenter);
		}

		if (labelRight != nullptr)
		{
			uint8_t textHeight = smallText ? gfx.setSmallFont() : gfx.setMediumFont();
			gfx.setTextColor(TFT_WHITE, -1);
			gfx.setCursor(x + width - gfx.textWidth(labelRight) - PADDING, y + height / 2 - textHeight / 2);
			gfx.print(labelRight);
		}

		uint8_t iconSize = smallIcon ? ICON_SIZE_SMALL : ICON_SIZE;

		if (iconCenter != nullptr)
		{
			gfx.drawBitmap(x + width / 2 - iconSize / 2, y + height / 2 - iconSize / 2, iconCenter, iconSize, iconSize, TFT_WHITE);
		}

		if (iconRight != nullptr)
		{
			gfx.drawBitmap(x + width - iconSize - PADDING, y + height / 2 - iconSize / 2, iconRight, iconSize, iconSize, TFT_WHITE);
		}
	}

	needsToDraw = false;
	bool pressed = previouslyTouching && rawTouchX < 0 && rawTouchY < 0;
	previouslyTouching = touchingButton;
	return pressed;
}
