

void loop() {


if (digitalRead(S1) == LOW) {
    
    R11();
    delay (10);
    int sensorValueLOW = analogRead(lightMeter);
    
            Serial.print ( "Light Value LOW: "); 
            Serial.println (sensorValueLOW);
    R15();   
    delay (10);     
    int sensorValueHIGH = analogRead(lightMeter);    
                    Serial.print ( "Light Value HIGH: "); 
                    Serial.println (sensorValueHIGH);
}


}

