

void loop() {

//    R11();

//    analogWrite(Solenoid1, 255);
//delay(20);
//analogWrite(Solenoid1, 0);


  if (digitalRead(S1) == LOW) {

  delay (50);
    
  unsigned long initialMillis = millis();  
  
  analogWrite(Solenoid1, 255);
  //analogWrite(Solenoid1, 0);  
 // timevalue[0] = {(millis() - initialMillis) };

//  unsigned long  sensor;

 

  analogWrite(Solenoid1, 0);

  for (int i = 1 ; i < numberOfSamples; i++)
  {
  mySamples[i].lightValue =  analogRead(lightMeter) ;
  mySamples[i].timeValue = {(millis() - initialMillis)} ;
 
  }
analogWrite(Solenoid1, 0);

 for (int i = 1 ; i < samples; i++)
{
Serial.print ("time: ");
Serial.print (mySamples[i].timeValue);
Serial.print (" ms  LOW: ");
Serial.println (mySamples[i].lightValue);

}

analogWrite(Solenoid1, 0);
Serial.println ("Shutter closed, capture ended");

delay (50);



  analogWrite(Solenoid1, 0);
 
//Serial.println ("DISABLE");
}
}
