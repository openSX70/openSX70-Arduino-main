

void loop() {

while ((Read_DS2408_PIO(1) == 1) && (Read_DS2408_PIO(2) == 1)) 
{
    R11();
  Write_DS2408_PIO(6,1);
/*  analogWrite(Solenoid1, 0);
Serial.print ("Shutter open: ");
Serial.println (analogRead(lightMeter));

Serial.print ("closing shutter...");
analogWrite(Solenoid1, 255);
delay (100);
Serial.print ("Shutter closed: ");
Serial.println (analogRead(lightMeter));
delay (100);
analogWrite(Solenoid1, 0);
*/

analogWrite(Solenoid1, 255);
//delay(20);
//analogWrite(Solenoid1, 0);


if (digitalRead(S1) == LOW) {

unsigned long initialMillis = millis();

analogWrite(Solenoid1, 255);
delay (10);

timevalue[0] = (millis() - initialMillis) ;

unsigned long  sensor;

sensor =  analogRead(lightMeter) ;

low[0] = sensor;

analogWrite(Solenoid1, 0);

for (int i = 1 ; i < samples; i++)
{


sensor =  analogRead(lightMeter) ;

low[i] = sensor ;

timevalue[i]  = (millis() - initialMillis) ;


}

 for (int i = 1 ; i < samples; i++)
{
Serial.print ("time: ");
Serial.print (timevalue[i]);
Serial.print (" ms  LOW: ");
Serial.println (low[i]);

}

analogWrite(Solenoid1, 0);
Serial.println ("Shutter closed, capture ended");

delay (50);



} 
{
  analogWrite(Solenoid1, 0);
  Write_DS2408_PIO(6,0);
 
}
}
}

