#ifndef SX70_PCB_H 
  #define SX70_PCB_H 
  #include "settings.h"

  #ifdef ARDUINO_AVR_PRO
    const uint8_t PIN_S1 = 12; //Sonar S1T S1 Trigger Redbutton
    const uint8_t PIN_S2 = A4;
    const uint8_t PIN_S3 = 6;
    const uint8_t PIN_S5 = 5;
    const uint8_t PIN_S8 = 8;
    const uint8_t PIN_S9 = 7;

    // Output pin definition
    const uint8_t PIN_SOL1 = 3; //orig 3 PD3(INT1)
    const uint8_t PIN_SOL2 = 11;

    const uint8_t PIN_MOTOR = 2;
    const uint8_t PIN_LED1 = 13; //LED RED
    const uint8_t PIN_LED2 = 10; //LED BLUE
    const uint8_t PIN_FF = A0;
    const uint8_t PIN_LM = A1;
    const uint8_t PIN_LM_RST = A2;
    const uint8_t PIN_FT = A3; //Sonar Flash Triggerd INPUT -- Cave Analoge Input only!
    const uint8_t PIN_GTD = A6; // Sonar gone that distance
    const uint8_t PIN_S1F = 9; //Sonar S1 Focus //Analog imput only
    const uint8_t PIN_S1F_FBW = 4; //engage SONAR prefocus
  #elif defined ARDUINO_GENERIC_G030K8TX
    const byte PIN_S1 = PB3; //Sonar S1T S1 Trigger Redbutton
    const byte PIN_S2 = PA2;
    const byte PIN_S3 = PC6;
    const byte PIN_S5 = PB2;
    const byte PIN_S8 = PA15;
    const byte PIN_S9 = PA12;

    // Output pin definition
    const byte PIN_SOL1 = PA8; //orig 3 PD3(INT1)
    const byte PIN_SOL2 = PA9;

    const byte PIN_MOTOR = PA10;
    const byte PIN_LED1 = PB8; //LED RED
    const byte PIN_LED2 = PB5; //LED BLUE
    const byte PIN_FF = PA5;
    const byte PIN_LM = PA3;
    const byte PIN_LM_RST = PA4;
    const byte PIN_FT = PA1; //Sonar Flash Triggerd INPUT -- Cave Analoge Input only!
    const byte PIN_GTD = PA0; // Sonar gone that distance
    const byte PIN_S1F = PB4; //Sonar S1 Focus //Analog imput only
    const byte PIN_S1F_FBW = PA11; //engage SONAR prefocus
  #endif

  void io_init();
#endif
