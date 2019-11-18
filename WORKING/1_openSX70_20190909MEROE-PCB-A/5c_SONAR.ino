#if SONAR

int WaitForFocus()
{
  long initialMillis; 
  long currentMillis;
  long focusMillis; //millis() when focus has been archived
  int countGTD = 0; //looking for a stable GTD value
  int lastGTD;
  int currentGTD;

  {
    initialMillis = millis(); //start clock to know how long it takes to focus
    lastGTD = analogRead(GTD); //assing a value 
    while (digitalRead(S1F) == HIGH && countGTD < 20) //looking for a stable GTD value at least 20 of them
    {
      delay (1); //wait for debouncing
      currentMillis = millis();
      currentGTD = analogRead(GTD);
      if (((currentGTD >= lastGTD) && (currentGTD - lastGTD < 15)) || ((currentGTD <= lastGTD) && (lastGTD - currentGTD < 15)))
        // looking for GTD values +- 15
        countGTD++;
      else
        countGTD = 0; //if I dont then I return 0
      lastGTD = currentGTD;  //
      //  Serial.print("S1F PRESSED: ");
      //  Serial.print (currentMillis-initialMillis);
      //Serial.print (" GTD: ");
      //  Serial.println (analogRead (GTD));

    }
    if (countGTD < 10)
      return 0;

    focusMillis = currentMillis - initialMillis;

    return focusMillis;

  }
}
#endif
