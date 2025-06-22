#include "game.h"
#include "constants.h"
#include <Arduino.h>
#include <LittleFS.h>

#define SETTINGS_FILE "/settings.json"

Game::Game() : dataObject(JsonDocument()) {}

Game::Game(JsonDocument dataObject) : dataObject(dataObject) {}

Game Game::readFromMemory()
{
    File file = LittleFS.open(SETTINGS_FILE, "r");
    if (file)
    {
        JsonDocument jsonDocument;
        DeserializationError deserializationError = deserializeJson(jsonDocument, file.readString());
        file.close();
        if (!deserializationError)
        {
            Serial.println("Successfully loaded game");
            return Game(jsonDocument);
        }
    }

    Serial.println("Failed to deserialize, creating a new game");
    return Game(JsonDocument());
}

void Game::reset()
{
    dataObject = JsonDocument();
    saveData();
    Serial.println("Game reset");
}

// Game settings

int32_t Game::getStartingBalance()
{
    return dataObject[SCHEMA_KEY_STARTING_BALANCE];
}

void Game::setStartingBalance(int32_t newStartingBalance)
{
    if (newStartingBalance != getStartingBalance())
    {
        dataObject[SCHEMA_KEY_STARTING_BALANCE] = newStartingBalance;
        saveData();
    }
}

uint8_t Game::getDecimalPlaces()
{
    return dataObject[SCHEMA_KEY_DECIMAL_PLACES];
}

void Game::setDecimalPlaces(uint8_t newDecimalPlaces)
{
    if (newDecimalPlaces != getDecimalPlaces())
    {
        dataObject[SCHEMA_KEY_DECIMAL_PLACES] = newDecimalPlaces;
        saveData();
    }
}

OverdraftHandling Game::getOverdraftHandling()
{
    return static_cast<OverdraftHandling>(dataObject[SCHEMA_KEY_OVERDRAFT_HANDLING].as<uint8_t>());
}

void Game::setOverdraftHandling(OverdraftHandling newOverdraftHandling)
{
    if (newOverdraftHandling != getOverdraftHandling())
    {
        dataObject[SCHEMA_KEY_OVERDRAFT_HANDLING] = static_cast<uint8_t>(newOverdraftHandling);
        saveData();
    }
}

// Player operations

void Game::addPlayer(const uint8_t *nfcId, uint8_t length, uint16_t color)
{
    if (getPlayerCount() < MAX_PLAYERS)
    {
        JsonDocument playerObject = JsonDocument();
        playerObject[SCHEMA_KEY_PLAYER_ID] = nfcIdToPlayerId(nfcId, length);
        playerObject[SCHEMA_KEY_PLAYER_BALANCE] = getStartingBalance();
        playerObject[SCHEMA_KEY_PLAYER_COLOR] = color;
        dataObject[SCHEMA_KEY_PLAYERS].add(playerObject);
        saveData();
    }
}

uint8_t Game::getPlayerCount()
{
    return dataObject[SCHEMA_KEY_PLAYERS].size();
}

int32_t Game::getPlayerBalance(uint8_t index)
{
    return dataObject[SCHEMA_KEY_PLAYERS][index][SCHEMA_KEY_PLAYER_BALANCE];
}

void Game::resetPlayerBalances()
{
    for (uint8_t i = 0; i < getPlayerCount(); i++)
    {
        dataObject[SCHEMA_KEY_PLAYERS][i][SCHEMA_KEY_PLAYER_BALANCE] = getStartingBalance();
    }
    saveData();
}

TransactionStatus Game::performTransaction(const uint8_t *nfcId, uint8_t length, int32_t amount)
{
    String playerId = nfcIdToPlayerId(nfcId, length);

    for (uint8_t i = 0; i < getPlayerCount(); i++)
    {
        if (playerId.equals(dataObject[SCHEMA_KEY_PLAYERS][i][SCHEMA_KEY_PLAYER_ID].as<String>()))
        {
            int32_t originalBalance = dataObject[SCHEMA_KEY_PLAYERS][i][SCHEMA_KEY_PLAYER_BALANCE];
            int32_t newBalance = originalBalance + amount;
            OverdraftHandling overdraftHandling = getOverdraftHandling();

            if (overdraftHandling == OverdraftHandling::BlockTransaction && newBalance < 0)
            {
                return TransactionStatus::InsufficientBalance;
            }

            if (overdraftHandling == OverdraftHandling::ClampToZero && newBalance < 0)
            {
                newBalance = 0;
            }

            if (originalBalance != newBalance)
            {
                dataObject[SCHEMA_KEY_PLAYERS][i][SCHEMA_KEY_PLAYER_BALANCE] = newBalance;
                saveData();
            }

            return TransactionStatus::Success;
        }
    }

    return TransactionStatus::TagNotFound;
}

// Utilities

void Game::saveData()
{
    File file = LittleFS.open(SETTINGS_FILE, "w");
    if (file)
    {
        serializeJson(dataObject, file);
        file.close();
    }
}

String Game::nfcIdToPlayerId(const uint8_t *nfcId, uint8_t length)
{
    String s = "";
    for (size_t i = 0; i < length; i++)
    {
        s += String(nfcId[i], HEX);
    }
    return s;
}
