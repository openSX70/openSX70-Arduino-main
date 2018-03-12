
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

              // Picture:
              //           int StructPicture--> continous picture counter  
              //           byte PackPicture --> within pack 1-8 counter 
              //           byte StructType  --> Type of picture (see table)

                                            // PictureType = 0 ---> MANUAL
                                            // PictureType = 1 ---> A100
                                            // PictureType = 2 ---> FLASH DONGLELESS
                                            // PictureType = 4 ---> FLASH F8 DONGLE 
                                            // PictureType = 6 ---> A600
              
              //           int StructSpeed  --> ShutterSpeed
              //           uint16_t StructLux --> Lux as read by BH170 just before picture-taking
               
