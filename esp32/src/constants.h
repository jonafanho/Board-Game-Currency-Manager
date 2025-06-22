#define MAX_PLAYERS 10

#define PADDING 16
#define KEYPAD_PADDING 4
#define BUTTON_COLOR 0x2104 // 0x222222

#define DECIMAL_OFFSET_FACTOR 100

// JSON Schema
#define SCHEMA_KEY_STARTING_BALANCE "startingBalance"
#define SCHEMA_KEY_DECIMAL_PLACES "decimalPlaces"
#define SCHEMA_KEY_OVERDRAFT_HANDLING "overdraftHandling"
#define SCHEMA_KEY_PLAYERS "players"
#define SCHEMA_KEY_PLAYER_ID "id"
#define SCHEMA_KEY_PLAYER_BALANCE "balance"
#define SCHEMA_KEY_PLAYER_COLOR "color"

// Keypad
#define KEYPAD_0_TEXT "0"
#define KEYPAD_1_TEXT "1"
#define KEYPAD_2_TEXT "2"
#define KEYPAD_3_TEXT "3"
#define KEYPAD_4_TEXT "4"
#define KEYPAD_5_TEXT "5"
#define KEYPAD_6_TEXT "6"
#define KEYPAD_7_TEXT "7"
#define KEYPAD_8_TEXT "8"
#define KEYPAD_9_TEXT "9"
#define KEYPAD_DECIMAL_TEXT "."

// Bottom buttons
#define OK_TEXT "OK"
#define CANCEL_TEXT "Cancel"
#define RESET_ALL_TEXT "Reset All"
#define ADD_PLAYER_TEXT "Add Player"
#define RESET_BALANCES_TEXT "Reset Balances"

// Welcome screen
#define WELCOME_TEXT_1 "Welcome!"
#define WELCOME_TEXT_2 "Touch screen to get started."

// Reset balances screen
#define RESET_BALANCES_TEXT_1 "Reset balances?"
#define RESET_BALANCES_TEXT_2 "All player balances will be"
#define RESET_BALANCES_TEXT_3 "set to the starting balance."

// Reset warning screen
#define RESET_TEXT_1 "Reset all?"
#define RESET_TEXT_2 "All game data and progress will be lost!"

// Settings
#define SETTINGS_TEXT "Game Settings"
#define PLAYERS_TEXT "Players"
#define STARTING_BALANCE_TEXT "Starting Balance"
#define DECIMAL_PLACES_TEXT "Decimal Places"
#define OVERDRAFT_HANDLING_TEXT "Overdraft"
#define OVERDRAFT_HANDLING_1_TEXT "Block"
#define OVERDRAFT_HANDLING_2_TEXT "Set To Zero"
#define OVERDRAFT_HANDLING_3_TEXT "Allow"
