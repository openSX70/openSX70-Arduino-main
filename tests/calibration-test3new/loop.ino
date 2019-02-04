

void loop() {

  if (digitalRead(S1) == LOW) {

  delay (50);  //some crappy debouncing
    
  
  analogWrite(Solenoid1, 255); //close the shutter

  delay (100); //wait for shutter to close!
  


  R15();
//  delay (100); //waiting...
  
Serial.print (" CLOSED read (SHOULD BE ZERO): ");
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
analogWrite(Solenoid1, 0);

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



  analogWrite(Solenoid1, 0);
 
//Serial.println ("DISABLE");
}
}
