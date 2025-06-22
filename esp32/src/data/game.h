#ifndef GAME_H
#define GAME_H

#include <ArduinoJson.h>

class Player;

enum class OverdraftHandling
{
	BlockTransaction,
	ClampToZero,
	Allow,
};

enum class TransactionStatus
{
	Success,
	TagNotFound,
	InsufficientBalance,
};

class Game
{
public:
	Game();
	static Game readFromMemory();
	void reset();

	int32_t getStartingBalance();
	void setStartingBalance(int32_t newStartingBalance);

	uint8_t getDecimalPlaces();
	void setDecimalPlaces(uint8_t newDecimalPlaces);

	OverdraftHandling getOverdraftHandling();
	void setOverdraftHandling(OverdraftHandling newOverdraftHandling);

	void addPlayer(const uint8_t *nfcId, uint8_t length, uint16_t color);
	uint8_t getPlayerCount();
	int32_t getPlayerBalance(uint8_t index);
	void resetPlayerBalances();
	TransactionStatus performTransaction(const uint8_t *nfcId, uint8_t length, int32_t amount);

private:
	JsonDocument dataObject;

	Game(JsonDocument dataObject);
	void saveData();
	static String nfcIdToPlayerId(const uint8_t *nfcId, uint8_t length);
};

#endif
