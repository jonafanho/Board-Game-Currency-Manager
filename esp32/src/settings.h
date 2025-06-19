#ifndef SETTINGS_H
#define SETTINGS_H

#include <ArduinoJson.h>

class Settings
{
public:
	Settings();
	JsonArray read(const char *key, const JsonArray defaultValue);
	const char *read(const char *key, const char *defaultValue);
	double read(const char *key, const double defaultValue);
	void write(JsonDocument input);
	static void clear();

private:
	JsonDocument jsonDocument;
	DeserializationError deserializationError;
};

#endif
