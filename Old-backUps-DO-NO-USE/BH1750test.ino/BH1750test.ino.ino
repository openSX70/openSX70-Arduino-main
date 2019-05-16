//Light meter software without pullup resistors using internal pullups.
//Peter Kooiman version
//2018/6/26


#include <Wire.h>
#include <BH1750.h>

BH1750 lightmeter;

const BH1750::Mode luxMode = BH1750::Mode::CONTINUOUS_HIGH_RES_MODE;
// BH1750_CONTINUOUS_HIGH_RES_MODE
// BH1750_CONTINUOUS_HIGH_RES_MODE_2
// BH1750_CONTINUOUS_LOW_RES_MODE
// BH1750_ONE_TIME_HIGH_RES_MODE
// BH1750_ONE_TIME_HIGH_RES_MODE_2
// BH1750_ONE_TIME_LOW_RES_MODE 

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Serial.println(F("Starting sensor..."));
  lightmeter.begin(luxMode, 3); // Inicializar BH1750
//  lightmeter.begin(luxMode, 3); // Inicializar BH1750
// (luxMode, RESOLUTION) --> 1 standard, 2 and 3 maximum resolution!  
    pinMode(A4, INPUT_PULLUP);
    pinMode(A5, INPUT_PULLUP);
}

void loop() {
  uint16_t lux = lightmeter.readLightLevel(); // Reading BH1750
  Serial.print(F("Lux:  "));
  Serial.print(lux);
  Serial.println(" lx");
  delay(500);
}
