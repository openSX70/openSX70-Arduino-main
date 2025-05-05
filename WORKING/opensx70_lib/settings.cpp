#include "settings.h"
#include "Arduino.h"
int currentPicture = 0; //why is this set to 1?
int mxshots = 0; //Multiple exposure counter
const uint8_t YDelay = 120;
const byte PowerDownDelay = 15; //time it takes to be fully closed
const byte PowerDown = 195; //max 255 = full power/POWERUP mode
int FD100 = round(.29*A100);
int FD600 = round(.29*A600);
byte lightmeterHelper = true;
// Added to remove the need to check for selector values prior to picture taking.

//int ShutterSpeed[] = {     25,   29,   32,    34,  39,   44,  64,   64,    175,  311, 609, 1109, POST, POSB, AUTO600, AUTO100 };
int ShutterSpeed[] = {     25,   29,   32,    34,  39,   44,  64,   64,    175,  311, 609, 1109, POST, POSB, AUTO600, AUTO100 };
//int ShutterSpeed[] = { 2000, 1000,  500,   250, 125,   60,  30,   15,      8,    4,   2,    1, T, B, AUTO600, AUTO100 };
//                        0     1      2      3    4      5    6     7       8     9    10   11

int flashDelay = 1; //new flash "system"
  // (These are default if not set, but changeable for convenience)

//int ShutterVariance[] = {  15,   10,   8,     6,    6,    6,   6,    6,      10,    20,   40,   40};
int ShutterVariance[] = {  15,   10,   8,     6,    6,    6,   6,    6,      10,    20,   40,   40};
//int ShutterSpeed[] = { 2000, 1000,  500,   250,  125,   60,  30,   15,      8,    4,   2,   1
//                          0     1     2      3    4     5    6     7       8     9    10   11

//int MinRange[] =       {   10,   23,   24,    28,   33,   38,  44,   50,    121,   200, 551,  1001};
int MinRange[] =       {   10,   23,   24,    28,   33,   38,  44,   50,    121,   200, 551,  1001};
//int ShutterSpeed[] = { 2000, 1000,  500,   250,  125,   60,  30,   15,      8,    4,   2,   1
//                          0     1     2      3    4     5    6     7       8     9    10   11

//int MaxRange[] =       {   25,   29,   32,    34,   39,   49,  89,   120,    199,  550, 1000, 1250};
int MaxRange[] =       {   25,   29,   32,    34,   39,   49,  89,   120,    199,  550, 1000, 1250};
//int ShutterSpeed[] = { 2000, 1000,  500,   250,  125,   60,  30,   15,      8,    4,   2,   1
//                          0     1     2      3    4     5    6     7       8     9    10   11
