#ifndef SX70_PCB_H 
  #define SX70_PCB_H 
  #include "settings.h"

  #if MEROE_Z_V1
    const uint8_t PIN_S1 = 12; //Sonar S1T S1 Trigger Redbutton
    const uint8_t PIN_S2 = A4;
    const uint8_t PIN_S3 = 8;
    // const uint8_t PIN_S4 = ;
    const uint8_t PIN_S5 = 7;
    const uint8_t PIN_S8 = A2;
    const uint8_t PIN_S9 = A1;

    // Output pin definition
    const uint8_t PIN_SOL1 = 3; //orig 3 PD3(INT1)
    const uint8_t PIN_SOL2 = 11;

    const uint8_t PIN_MOTOR = A3;
    const uint8_t PIN_LED1 = 13; //LED RED
    const uint8_t PIN_LED2 = 10; //LED BLUE
    const uint8_t PIN_FF = A6;
    const uint8_t PIN_LM = A0;
    const uint8_t PIN_LM_RST = 2;
    #if SONAR
      const uint8_t PIN_GTD = A7; // Sonar gone that distance
      const uint8_t PIN_S1F = 9; //Sonar S1 Focus //Analog imput only
      //const uint8_t PIN_FT = A7; //Sonar Flash Triggerd INPUT -- Cave Analoge Input only!
      const uint8_t PIN_S1F_FBW = 4; //engage SONAR prefocus
    #endif
  #elif MEROE_Z_V2
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
    #if SONAR
      const uint8_t PIN_GTD = A6; // Sonar gone that distance
      const uint8_t PIN_S1F = 9; //Sonar S1 Focus //Analog imput only
      const uint8_t PIN_S1F_FBW = 4; //engage SONAR prefocus
    #endif
  #endif

  void io_init();
#endif
