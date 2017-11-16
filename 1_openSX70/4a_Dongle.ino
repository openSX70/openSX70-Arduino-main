void Dongle(int DongleSlot) 
    {
      
    Serial.println ("Dongle");
  
    if (takePicture == true)
    {
                        shutterCLOSE (); 
                  Serial.println ("close");
                  delay (500);
                  shutterOPEN();
                  Serial.println ("open");
                  delay (500);
                  takePicture == false;                  
    }
    /*
    byte Selector = DongleSlot;

    if (takePicture == true && Selector ==  1 && shots == 0)          //NORMAL OPERATION
     {
            
              shutterCLOSE (); 
                  Serial.println ("close");

  delay (500);
              Click (); 
                  Serial.println ("click");

  delay (500);
              shutterOPEN();
                  Serial.println ("open");

     }
     */
    }

/* COMMENTED FOR TESTS    
    if (takePicture == true && Selector ==  1 && shots == 0)          //NORMAL OPERATION
     {

          shutterCLOSE ();  // I CLOSE THE SHUTTER
                
        //delay (20);     //I wait? Is this really necessary???
        //Motor Starts: MIRROR COMES UP!!!

        mirrorUP();

         while (digitalRead(S3) != HIGH)            //waiting for S3 to OPEN
           ;
                
        delay (40);                               //S3 is now open start Y-delay (40ms)

        Click ();                                // NOW I am going to take the picture: SMILE!
 

        delay (200);                             //AGAIN is this delay necessary?
        
        mirrorDOWN ();                          //Motor starts, let bring the mirror DOWN

        delay (200);                             //AGAIN is this delay necessary?

        shutterOPEN();
       

     }  //end of if (digitalRead(S1) == LOW && digitalRead(Selector) ==  HIGH && shots == 0)          //NORMAL OPERATION
                  
     // CASE 2 DOUBLE EXPOSURE FIRST SHOT: MIRROR DOWN AND FIRST PICTURE (CLICK: SHUTTER OPERATION REMAINING CLOSED)           
     if (digitalRead(S1) == LOW && Selector ==  0 && shots == 0)    //DOUBLE EXPOSURE and RED BUTTON PRESSED (S1) FIRST SHOT!!!!
      {

          shutterCLOSE ();  // I CLOSE THE SHUTTER

                                          // delay (20);     //I wait? Is this really necessary???
                                            //Motor Starts: MIRROR COMES UP!!!
          mirrorUP();

          while (digitalRead(S3) != HIGH)            //waiting for S3 to OPEN
            ;
          //S3 is now open start Y-delay (40ms)
          delay (40);

          Click ();                 // NOW I am going to take the picture: SMILE!

          delay (1000);            //debounce will go here
          shots++;


          } // end of if if (digitalRead(S1) == LOW && digitalRead(Selector) ==  LOW && shots == 0)    //DOUBLE EXPOSURE and RED BUTTON PRESSED (S1) FIRST SHOT!!!!

         // CASE 3 DOUBLE EXPOSURE ULTERIOR SHOTS: NO MOTOR OPERATION JUST PICTURE (CLICK: SHUTTER OPERATION REMAINING CLOSED)  
         if (digitalRead(S1) == LOW && Selector ==  0 && shots >= 1)    //DOUBLE EXPOSURE and RED BUTTON PRESSED (S1) ULTERIOR SHOTS!!!!
          {
          Click();
          
          delay (1000); //debounce will go here
          
          shots++;

          } // end of  if (digitalRead(S1) == LOW && digitalRead(Selector) ==  LOW && shots >= 1)    //DOUBLE EXPOSURE and RED BUTTON PRESSED (S1) ULTERIOR SHOTS!!!!

          // CASE 4 PICTURE EXPULSION AFTER DOUBLE EXPOSURE: MIRROR DOWN AND SHUTTER OPENING (NO PICTURE TAKEN)
         if (digitalRead(S1) == HIGH && digitalRead(Selector) ==  HIGH && shots >= 1)  //Was in DOUBLE EXPOSURE MODE but NOW is back to NORMAL
          {
             shots = 0;
 
            mirrorDOWN ();

            delay (200);                             //AGAIN is this delay necessary?

            shutterOPEN();    

           }   // end of if (digitalRead(S1) == HIGH && digitalRead(Selector) ==  LOW && shots > 1)  //Was in DOUBLE EXPOSURE MODE but NOW is back to NORMAL
   

    }// end of if (digitalRead(S8) == LOW && digitalRead(S9) == LOW)
     // END OF STATE 3: NORMAL OPERATION
*/

