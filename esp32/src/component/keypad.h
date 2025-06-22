#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>
#include <Arduino.h>
#include "button.h"
#include "hardware/display.h"

class Keypad
{
public:
	Keypad(uint8_t maxDigits, uint8_t decimalPlaces);
	int32_t show();

private:
	static const uint8_t ROWS = 4;
	static const uint8_t COLUMNS = 4;
	static const uint16_t WIDTH = SCREEN_HEIGHT;
	static const uint16_t BUTTON_SIZE = WIDTH / COLUMNS;
	static const uint16_t HEIGHT = BUTTON_SIZE * ROWS;
	static const uint16_t BUTTONS_START_Y = SCREEN_WIDTH - HEIGHT;

	const uint8_t maxDigits;
	const uint8_t decimalPlaces;

	Button button0;
	Button button1;
	Button button2;
	Button button3;
	Button button4;
	Button button5;
	Button button6;
	Button button7;
	Button button8;
	Button button9;
	Button buttonDecimal;
	Button buttonBackspace;
	Button buttonCancel;
	Button buttonEnter;

	String currentInput = "";

	void typeDigit(char key);
	static const Button createButton(const char *label, const uint8_t *icon, uint8_t x, uint8_t y, uint8_t width, uint8_t height);
};

#endif
