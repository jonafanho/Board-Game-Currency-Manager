#include "keypad.h"
#include "constants.h"
#include "icons.h"

Keypad::Keypad(uint8_t maxDigits, uint8_t decimalPlaces)
	: maxDigits(maxDigits),
	  decimalPlaces(decimalPlaces),
	  button0(createButton(KEYPAD_0_TEXT, nullptr, 0, 3, decimalPlaces > 0 ? 2 : 3, 1)),
	  button1(createButton(KEYPAD_1_TEXT, nullptr, 0, 2, 1, 1)),
	  button2(createButton(KEYPAD_2_TEXT, nullptr, 1, 2, 1, 1)),
	  button3(createButton(KEYPAD_3_TEXT, nullptr, 2, 2, 1, 1)),
	  button4(createButton(KEYPAD_4_TEXT, nullptr, 0, 1, 1, 1)),
	  button5(createButton(KEYPAD_5_TEXT, nullptr, 1, 1, 1, 1)),
	  button6(createButton(KEYPAD_6_TEXT, nullptr, 2, 1, 1, 1)),
	  button7(createButton(KEYPAD_7_TEXT, nullptr, 0, 0, 1, 1)),
	  button8(createButton(KEYPAD_8_TEXT, nullptr, 1, 0, 1, 1)),
	  button9(createButton(KEYPAD_9_TEXT, nullptr, 2, 0, 1, 1)),
	  buttonDecimal(createButton(KEYPAD_DECIMAL_TEXT, nullptr, 2, 3, 1, 1)),
	  buttonBackspace(createButton(nullptr, iconBackspace, 3, 0, 1, 1)),
	  buttonCancel(createButton(nullptr, iconCancel, 3, 1, 1, 1)),
	  buttonEnter(createButton(nullptr, iconCheck, 3, 2, 1, 2)) {}

int32_t Keypad::show()
{
	typeDigit(' ');

	while (true)
	{
		lgfx::touch_point_t touchPoint;
		gfx.getTouch(&touchPoint);
		int16_t rawTouchX = touchPoint.x;
		int16_t rawTouchY = touchPoint.y;

		if (button0.tick(rawTouchX, rawTouchY))
		{
			typeDigit('0');
		}
		if (button1.tick(rawTouchX, rawTouchY))
		{
			typeDigit('1');
		}
		if (button2.tick(rawTouchX, rawTouchY))
		{
			typeDigit('2');
		}
		if (button3.tick(rawTouchX, rawTouchY))
		{
			typeDigit('3');
		}
		if (button4.tick(rawTouchX, rawTouchY))
		{
			typeDigit('4');
		}
		if (button5.tick(rawTouchX, rawTouchY))
		{
			typeDigit('5');
		}
		if (button6.tick(rawTouchX, rawTouchY))
		{
			typeDigit('6');
		}
		if (button7.tick(rawTouchX, rawTouchY))
		{
			typeDigit('7');
		}
		if (button8.tick(rawTouchX, rawTouchY))
		{
			typeDigit('8');
		}
		if (button9.tick(rawTouchX, rawTouchY))
		{
			typeDigit('9');
		}
		if (decimalPlaces > 0 && buttonDecimal.tick(rawTouchX, rawTouchY))
		{
			typeDigit('.');
		}
		if (buttonBackspace.tick(rawTouchX, rawTouchY))
		{
			typeDigit('-');
		}
		if (buttonCancel.tick(rawTouchX, rawTouchY))
		{
			return -1;
		}
		if (buttonEnter.tick(rawTouchX, rawTouchY))
		{
			return round(currentInput.toDouble() * DECIMAL_OFFSET_FACTOR);
		}
	}
}

void Keypad::typeDigit(char key)
{
	uint8_t length = currentInput.length();
	bool isEmpty = length == 0;
	int8_t decimalIndex = currentInput.indexOf(".");

	if (key >= '0' && key <= '9')
	{
		if ((key > '0' || !isEmpty) && (decimalIndex < 0 && length < maxDigits || length - decimalIndex <= decimalPlaces))
		{
			currentInput += key;
		}
	}
	else if (key == '.' && decimalIndex < 0)
	{
		if (isEmpty)
		{
			currentInput = "0";
		}
		currentInput += '.';
	}
	else if (key == '-')
	{
		if (length > 0)
		{
			currentInput.remove(length - 1);
		}
		if (currentInput.equals("0"))
		{
			currentInput = "";
		}
	}

	uint8_t newLength = currentInput.length();

	if (key == ' ' || newLength != length)
	{
		uint8_t textHeight = gfx.setLargeFont();
		gfx.fillRect(0, BUTTONS_START_Y - textHeight, WIDTH, textHeight, TFT_BLACK);
		String renderText = newLength == 0 ? "0" : currentInput;
		gfx.setTextColor(newLength == 0 ? TFT_DARKGRAY : TFT_WHITE);
		gfx.setCursor(WIDTH - gfx.textWidth(renderText), BUTTONS_START_Y - textHeight);
		gfx.print(renderText);
	}
}

const Button Keypad::createButton(const char *label, const uint8_t *icon, uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
	return Button(nullptr, label, nullptr, icon, nullptr, x * BUTTON_SIZE + KEYPAD_PADDING, BUTTONS_START_Y + y * BUTTON_SIZE + KEYPAD_PADDING, width * BUTTON_SIZE - KEYPAD_PADDING * 2, height * BUTTON_SIZE - KEYPAD_PADDING * 2, BUTTON_COLOR, false, false);
}
