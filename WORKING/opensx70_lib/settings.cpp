#include "settings.h"
#include "Arduino.h"

int mxshots = 0; //Multiple exposure counter
const uint8_t YDelay = 100;
const byte PowerDownDelay = 15; //time it takes to be fully closed
const byte PowerDown = 77; //max 255 = full power/POWERUP mode

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

byte lightmeterHelper = true;

int ShutterSpeed[] = {     22,   24,   25,    27,  31,   38,  56,   90,    148,  273, 523, 1023, POST, POSB, AUTO600, AUTO100 };

//int ShutterSpeed[] = { 2000, 1000,  500,   250, 125,   60,  30,   15,      8,    4,   2,    1, T, B, AUTO600, AUTO100 };
//                          0     1     2      3    4     5    6     7       8     9    10   11
//OPTION line above are the wheel "raw" speeds (have to keep in mind smaller time = smaller aperture -> inefficient shutter)
// this are the "SLOTS":+
//int ShutterSpeed[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F };
//int ShutterSpeed[] = { EV17, EV16, EV15, EV14, EV13, EV12, EV11.5, EV11, EV10.5, EV10, EV9, EV8, AUTO600, AUTO100, T, B };
//int ShutterSpeed[] = { 2000, 1000,  500,   260, 130,   60,     45,   30,     25,   20,   8,   4, AUTO600, AUTO100, T, B };
//int ShutterSpeed[] = {   17,   20,   23,    25,  30,   35,     45,   55,     68,  102, 166, 302, AUTO600, AUTO100, POST, POSB };    //uDONGLE SETTINGS
//int ShutterSpeed[] = { 2000, 1000,  500,   250, 125,   60,     30,   15,      8,    4,   2,   1, T, B, AUTO600, AUTO100 };
//int ShutterSpeed[] = {   16,   20,   23,    25,  30,   35,     55,   55,    166,  302, 600, 1100, POST, POSB, AUTO600, AUTO100 };  //SANTI DONGLE WHEEL
// to change the speed in the slot position just change the number corresponding.
int flashDelay = 1; //new flash "system"
  // (These are default if not set, but changeable for convenience)

int ShutterVariance[] = {  6,   6,   6,     6,    6,    6,   6,    6,      10,    20,   40,   40};
//int ShutterSpeed[] = { 2000, 1000,  500,   250,  125,   60,  30,   15,      8,    4,   2,   1
//                          0     1     2      3    4     5    6     7       8     9    10   11


int MinRange[] =       {   19,   26,   29,    33,   37,   41,  45,   65,     121,   176, 312,  610};
//int ShutterSpeed[] = { 2000, 1000,  500,   250,  125,   60,  30,   15,      8,    4,   2,   1
//                          0     1     2      3    4     5    6     7        8     9    10   11

int MaxRange[] =       {   25,   28,   32,    36,   40,   44,  64,   120,    175,  311, 609, 1109};
//int ShutterSpeed[] = { 2000, 1000,  500,   250,  125,   60,  30,   15,      8,    4,   2,   1
//                          0     1     2      3    4     5    6     7       8     9    10   11