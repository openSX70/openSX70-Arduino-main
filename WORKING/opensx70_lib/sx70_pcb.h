#ifndef SX70_PCB_H 
  #define SX70_PCB_H 
  #include "settings.h"

  #if MEROE_PCB
    const uint8_t PIN_S1 = 12; //Sonar S1T S1 Trigger Redbutton
    const uint8_t PIN_S2 = A4;
    const uint8_t PIN_S3 = 8;
    // const uint8_t PIN_S4 = ;
    const uint8_t PIN_S5 = 7;
    const uint8_t PIN_S8 = A1;
    const uint8_t PIN_S9 = A0;
    
    // Output pin definition
    const uint8_t PIN_SOL1 = 3; //orig 3 PD3(INT1)
    const uint8_t PIN_SOL2 = 11;
    
    const uint8_t PIN_MOTOR = A2;
    const uint8_t PIN_LED1 = 13; //LED RED
    const uint8_t PIN_LED2 = A3; //LED BLUE
    const uint8_t PIN_FF = 4;
    const uint8_t PIN_OE = 9;         //Pin 13 on Meroe2  (PB1-OC1A)
    const uint8_t TCS3200_S1_Pin = 2; //Pin 32 on Meroe2  (PD2-INT0)
    const uint8_t TCS3200_S3_Pin = 6; //Pin 10 on Meroe2 (PD6-AIN0)

  #elif LAND_PCB
    const uint8_t PIN_S1 = 12; //Sonar S1T S1 Trigger Redbutton
    const uint8_t PIN_S2 = A4;
    const uint8_t PIN_S3 = 8;
    // const uint8_t PIN_S4 = ;
    const uint8_t PIN_S5 = 7;
    const uint8_t PIN_S8 = A1;
    const uint8_t PIN_S9 = A0;
    
    // Output pin definition
    const uint8_t PIN_SOL1 = 3; //orig 3 PD3(INT1)
    const uint8_t PIN_SOL2 = 11;
    
    const uint8_t PIN_MOTOR = A2;
    const uint8_t PIN_LED1 = 13; //LED RED
    const uint8_t PIN_LED2 = A3; //LED BLUE
    const uint8_t PIN_FF = 4;
    const uint8_t PIN_OE = 9;         //Pin 13 on Meroe2  (PB1-OC1A)
    const uint8_t TCS3200_S1_Pin = 2; //Pin 32 on Meroe2  (PD2-INT0)
    const uint8_t TCS3200_S3_Pin = 6; //Pin 10 on Meroe2 (PD6-AIN0)

  #elif SONAR_PCB
    const uint8_t PIN_S1 = 12; //Sonar S1T S1 Trigger Redbutton
    const uint8_t PIN_S2 = A4;
    const uint8_t PIN_S3 = 8;
    // const uint8_t PIN_S4 = ;
    const uint8_t PIN_S5 = 7;
    const uint8_t PIN_S8 = A1;
    const uint8_t PIN_S9 = A0;
    
    // Output pin definition
    const uint8_t PIN_SOL1 = 3; //orig 3 PD3(INT1)
    const uint8_t PIN_SOL2 = 11;
    
    const uint8_t PIN_MOTOR = A2;
    const uint8_t PIN_LED1 = 13; //LED RED
    const uint8_t PIN_LED2 = A3; //LED BLUE
    const uint8_t PIN_FF = 4;
    const uint8_t TCS3200_S1_Pin = 9;
    const uint8_t TCS3200_S3_Pin = 6;
    #if SONAR
      const uint8_t PIN_GTD = A5; // Sonar gone that distance
      const uint8_t PIN_S1F = 10; //Sonar S1 Focus //Analog imput only
      const uint8_t PIN_FT = A7; //Sonar Flash Triggerd INPUT -- Cave Analoge Input only!
      const uint8_t PIN_S1F_FBW = 2; //engage SONAR prefocus
    #endif

  #elif EDWIN_PCB
    const uint8_t PIN_S1 = 12; //Sonar S1T S1 Trigger Redbutton
    const uint8_t PIN_S2 = A4;
    const uint8_t PIN_S3 = 8;
    // const uint8_t PIN_S4 = ;
    const uint8_t PIN_S5 = 7;
    const uint8_t PIN_S8 = A1;
    const uint8_t PIN_S9 = A0;
    
    // Output pin definition
    const uint8_t PIN_SOL1 = 3; //orig 3 PD3(INT1)
    const uint8_t PIN_SOL2 = 11;
    
    const uint8_t PIN_MOTOR = A2;
    const uint8_t PIN_LED1 = 13; //LED RED
    const uint8_t PIN_LED2 = A3; //LED BLUE
    const uint8_t PIN_FF = 4;

  #elif ALPHA2_PCB
    // Input pin definition
    const uint8_t PIN_S1 = 12;
    const uint8_t PIN_S2 = 2;
    const uint8_t PIN_S3 = 8;
    // const uint8_t PIN_S4 = ;
    const uint8_t PIN_S5 = 7;
    const uint8_t PIN_S8 = A1;
    const uint8_t PIN_S9 = A0;

    // Output pin definition
    const uint8_t PIN_SOL1 = 3;
    const uint8_t PIN_SOL2 = 11;
    const uint8_t PIN_FF = 4;
    const uint8_t PIN_MOTOR = A2;

    const uint8_t PIN_LED1 = 13;
    const uint8_t PIN_LED2 = A3;

  #elif ECM_PCB
    const uint8_t PIN_FPIN = 6;
    const uint8_t PIN_S1 = 12; //Sonar S1T S1 Trigger Redbutton
    const uint8_t PIN_S2 = A4;
    const uint8_t PIN_S3 = 8;
    // const uint8_t PIN_S4 = ;
    const uint8_t PIN_S5 = 7;
    const uint8_t PIN_S8 = A1;
    const uint8_t PIN_S9 = A0;
    
    // Output pin definition
    const uint8_t PIN_SOL1 = 3; //orig 3 PD3(INT1)
    const uint8_t PIN_SOL2 = 11;
    
    const uint8_t PIN_MOTOR = A2;
    const uint8_t PIN_LED1 = 13; //LED RED
    const uint8_t PIN_LED2 = A3; //LED BLUE
    const uint8_t PIN_FF = 4;

    const uint8_t PIN_SOL1LOW = 20; //SOL1 low power on ECM_PCB
    const uint8_t PIN_SOL2LOW = 21; //SOL2 low power on ECM_PCB
    const uint8_t PIN_ISO=A6;       //MANUAL    iSO SELECTION
    const uint8_t PIN_FT = A7;      //Sonar Flash Triggerd INPUT -- Cave Analoge Input only!
    //const uint8_t TCS3200_S1_Pin = 9;
    const uint8_t TCS3200_OE_Pin = 9;         //Pin 13 on Meroe2  (PB1-OC1A)
	      
    #if SONAR
      const uint8_t PIN_GTD = A5; // Sonar gone that distance
      const uint8_t PIN_S1F = 10; //Sonar S1 Focus //Analog imput only
      const uint8_t PIN_S1F_FBW = 2; //engage SONAR prefocus
	  #endif
  #endif

  void io_init();
#endif
