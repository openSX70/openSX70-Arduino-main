
// openSX70 code with tabs
// HELLO SX70!!!!!
//Remember I am not a programmer, this is probably a mess. Use at your own risk.
//You can do whatever you want for personal use but NOT for commercial purposes.
//As for the electronics are really simple but will try to make an schematic when I find the time
//This will work with "normal" arduinos (UNO etc...)
//You have been warned!
//I have "stolen" code from all over, if not credited I apologize.
//this is the first "proper" GitHub code
// 16/11/2017 Fixed many things. Optional MOTOR or SHUTTER for tests only shutter and only MOTOR. BUG: with double exposure with Switch 1 = 0 (Read_DS2408_PIO(1) = 1 (ON). For some reason does two pictures in case 2.
// 10/03/2018 First working camera?
// 23/03/2018 EEPROM working, eepromUpdate function, pos B and pos T working. 
// 27/03/2018 Prototype# Len's and Bellows flash! Newer speeds! Testing BH1750 BH1750_CONTINUOUS_LOW_RES_MODE.
// 28/03/2018 Ordered boards today!!! fixed timer led on dongle.
//  3/07/2018 Fix for non-blinking LED!!! (thanks Pitsie!)
//  7/08/2018 Changed some shutter speed timings (faster "0" position) and the redbutton timings.
//  15/10/2018 Removed all related to BH1750 and moved to photodiode in new PCB "ALPHA"

//  7/12/2018 ALPHA

              // FIRST I ASSIGN THE Sx SWITCHES OF THE SX70 TO PINS ON THE ARDUINO
              // THESE ARE INPUTS
              // SX70-ARDUINO LOGICAL EQUIVALENTS
              // S1: LOW = CLOSED
              // S3: LOW = CLOSED
              // S5: LOW = CLOSED
              // S8: HIGH = CLOSED
              // S9: HIGH = CLOSED

              //EEPROM CHEATSHEET
              //  0-> S  initialization purposes
              //  1-> X  initialization purppses
              //  3-> ActualPicture --> From the beggining absolute picture counter
              //  4-> CurrentPicture --> 1-8 "Pack" picture counter
              // 10-> eeAddress Where to write next
              // 13-> ActualPicture --> Counter from begining
              //ActualPicture
              //CurrentPicture 

              // Picture:MyPicture
              //           int StructPicture:ActualPicture --> continous picture counter  
              //           byte PackPicture:CurrentPicture --> within pack 1-8 counter 
              //           byte StructType:PictureType  --> Type of picture (see table)

                                            // PictureType = 0 ---> MANUAL
                                            // PictureType = 1 ---> A100
                                            // PictureType = 2 ---> FLASH DONGLELESS
                                            // PictureType = 4 ---> FLASH F8 DONGLE 
                                            // PictureType = 5 ---> FLASH DONGLE 
                                            // PictureType = 6 ---> A600
                                            // PictureType = 7 ---> Bulb
                                            // PictureType = 8 ---> T
                                            
              
              //           int StructSpeed:ShutterSpeed  --> ShutterSpeed


//  23/12/2018 MAJOR CLEAN UP!!!!!!!!!!!!!!!!!!! (openSX70_ALPHA-CLEAN-NEW)

                 
