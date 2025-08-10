#include "settings.h"
#include "Arduino.h"

const uint8_t YDelay = 100;
const byte PowerDownDelay = 15; //time it takes to be fully closed

// *** FILL FLASH ***
// Explanation for these numbers. Concept is pulled directly from fill flash description in the alpha repair supplement.
// FD values are 1/3 of ISO magic numbers
// FF values are 1.3x the FD numbers
// FD (Flash Delay) value is the value the meter must hit prior to the flash firing. If integration time takes longer than 56ms (Flash_Max_Time), fire the flash regardless.
// FF (Flash Fired) value is the value the meter must hit before the shutter closes. This also has a timeout but is a much shorter 12ms (Flash_Capture_Max_Time).
// I am hard coding these values as the magic number is no longer changing per board.
int FD100 = 341;
int FF100 = 443;
int FD600 = 85;
int FF600 = 111;
uint8_t retryLimit = 3;

byte lightmeterHelper = true;

// values are currently in ms, may move to us
int ShutterSpeed[] = {     23,   27,   32,    34,  39,   44,  56,   64,    125,  250, 500, 1000, POST, POSB, AUTO600, AUTO100 };
//int ShutterSpeed[] = { 2000, 1000,  500,   250, 125,   60,  30,   15,      8,    4,   2,    1, T, B, AUTO600, AUTO100 };
//                          0     1     2      3    4     5    6     7       8     9    10   11
