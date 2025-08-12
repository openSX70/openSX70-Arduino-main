#ifndef SX70_PCB_H 
#define SX70_PCB_H 
#ifdef ARDUINO_GENERIC_G030K8TX
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
    const byte USART_RX = PB7;
    const byte USART_TX = PB6;
#endif

void io_init();
#endif
