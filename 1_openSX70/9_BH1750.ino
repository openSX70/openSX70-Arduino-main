/*
#include <Wire.h>
#include <BH1750.h>

BH1750 lightmeter;

const byte luxMode = BH1750_CONTINUOUS_HIGH_RES_MODE;
// BH1750_CONTINUOUS_HIGH_RES_MODE
// BH1750_CONTINUOUS_HIGH_RES_MODE_2
// BH1750_CONTINUOUS_LOW_RES_MODE
// BH1750_ONE_TIME_HIGH_RES_MODE
// BH1750_ONE_TIME_HIGH_RES_MODE_2
// BH1750_ONE_TIME_LOW_RES_MODE

void setup() {
  Serial.begin(9600);
  Serial.println(F("Inicializando sensor..."));
  lightmeter.begin(luxMode); // Inicializar BH1750
}

void loop() {
  uint16_t lux = lightmeter.readLightLevel(); // Lectura del BH1750
  Serial.print(F("Iluminancia:  "));
  Serial.print(lux);
  Serial.println(" lx");
  delay(500);
}
*/
