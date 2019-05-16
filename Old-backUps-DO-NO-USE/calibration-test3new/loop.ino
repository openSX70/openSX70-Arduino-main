

void loop() {

  if (digitalRead(S1) == LOW) {

  delay (500);  //some crappy debouncing
    

  R15();
    
  analogWrite(Solenoid1, 255); //close the shutter

  delay (100); //wait for shutter to close!
  


//  delay (100); //waiting...
  
Serial.print (" CLOSED read (SHOULD BE ZERO): ");

for (int i= 0; i<100;i++)
{
analogRead(lightMeter);
}
Serial.println (analogRead(lightMeter));

  
 
unsigned long initialMillis = millis();  //set the mark to start counting time in ms

  analogWrite(Solenoid1, 0); //open the shutter NOW

  for (int i = 1 ; i < numberOfSamples; i++)
  {
  mySamples[i].lightValue =  analogRead(lightMeter) ;
  mySamples[i].timeValue = {(millis() - initialMillis)} ;
  delayMicroseconds (10000);  
  //delay (1);
  }
analogWrite(Solenoid1, 0); //OPEN SHUTTER

 for (int i = 1 ; i < numberOfSamples; i++)
{
Serial.print ("time: ");
Serial.print (mySamples[i].timeValue);
Serial.print (" ms  LightMeter: ");
Serial.println (mySamples[i].lightValue);

}

analogWrite(Solenoid1, 0); //shutter open please please!!

Serial.println ("Shutter closed, capture ended");

  delay (50);
  analogWrite(Solenoid1, 0); //just in case open shutter
 
//Serial.println ("DISABLE");
}
}
