/*
	**the openSX70 project**
	
   It is many things at once, but simply put, openSX70 is an open source 
  (hardware and software) project that aims to take the SX70 beyond what
  is possible now in a cheap and non destructive way.
	https://opensx70.com/
	
	https://github.com/openSX70
	
  As a legal reminder please note that the code and files is under Creative Commons 
  "Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)" is free and open 
  for hobbyist NON-COMMERCIAL USE.
	https://creativecommons.org/licenses/by-nc/4.0/
	
	You are free to:
	Share — copy and redistribute the material in any medium or format
	Adapt — remix, transform, and build upon the material
	The licensor cannot revoke these freedoms as long as you follow the license terms.
	Under the following terms:
	Attribution — You must give appropriate credit, provide a link to the license, 
	and indicate if changes were made. You may do so in any reasonable manner, but 
	not in any way that suggests the licensor endorses you or your use.

	NonCommercial — You may not use the material for commercial purposes.

	No additional restrictions — You may not apply legal terms or technological 
	measures that legally restrict others from doing anything the license permits.
	
	Notices:
	You do not have to comply with the license for elements of the material in the 
	public domain or where your use is permitted by an applicable exception or limitation.

	No warranties are given. The license may not give you all of the permissions necessary 
	for your intended use. For example, other rights such as publicity, privacy, or moral r
	ights may limit how you use the material.
  
  
*/
#ifndef settings_h
#define settings_h

#include "Arduino.h"

#define GREEN 6
#define RED 7

#define LIGHMETER_HELPER 1
//LIGHMETER_HELPER 0  NO helper
//LIGHMETER_HELPER 1 for VF helper
//LIGHMETER_HELPER 2  FUTURE dongle helper

#define SIMPLEDEBUG 0
//#define TSL237T
//#define TSL235R
//
#define TCS3200


//NOT IN USE
//#define METER_CHIP TSL237T
//#define METER_CHIP TSL235R
//#define METER_CHIP TCS3200

//S1Logic LOW = Alphas, Meroë, Edwin
//S1Logic HIGH = Sonar 
#define S1Logic HIGH

extern const uint8_t YDelay;

extern const byte PowerDownDelay; //time it takes to be fully closed
extern const byte PowerDown; //max 255 = full power/POWERUP mode


#define DEBOUNCECOUNT 5

      

 
extern int currentPicture;
extern int prevDongle ;

#define DEFAULT_ISO ISO_600

#define ISO_SX70 125
#define ISO_600 640




#define EE_ADD_CUR   4
 //-> CurrentPicture --> 1-8 "Pack" picture counter
#define EE_ADD_ADD   30
 //-> eeAddress Where to write next
#define EE_ADD_PIC   13
 //-> ActualPicture --> Counter from begining
#define EE_ADD_ISO   20
//-> camera current ISO (dongleless)

//OPTION REGARDING SELECTOR WHEEL


//AnalogueDongle
enum positions_t {POST = -100, POSB, AUTO600, AUTO100 };//ANALOGUEWORKS
//uDONGLE
//enum positions_t {AUTO600 = -100, AUTO100, POST, POSB}; //uDONGLE
/*
  TO CLARIFY:

    uDongle               AnalogueDongle
    AUTO600 = - 100       POST = -100
    AUTO100 = -99         POSB = -99
    POST = -98            AUTO600 = - 98
    POSB = -97            AUTO100 = - 97
*/



//void BlinkISO ();
extern int ShutterConstant;

//OPTION

extern int ShutterSpeed[];


//reduced speeds from 25 (slot5) to compensate flash firing


//flashDelay is the time it takes to fire the flash (even if none is connected) when slow shutterspeeds

extern int flashDelay; //new flash "system"

//this speed and SLOWER will trigger flash

extern int shots; //Multiple exposure counter
/*
struct PictureData
{
  uint16_t StructPicture;         //total count of pictures since init
  uint16_t StructPackPicture;          //pic count within this pack
  uint16_t StructType;           //picture type

  // PictureType = 0 ---> MANUAL
  // PictureType = 1 ---> A100
  // PictureType = 2 ---> FLASH DONGLELESS
  // PictureType = 4 ---> FLASH F8 DONGLE
  // PictureType = 6 ---> A600
  // PictureType = 7 ---> BULB
  // PictureType = 8 ---> T
  // PictureType = +10 ---> MULTIPLE EXPOSURE
  // PictureType = +100 ---> TIMER DELAY
  
  
  uint16_t StructSpeed;           //shutter speed
  
};
*/
              //EEPROM CHEATSHEET
              //  0-> S  initialization purposes
              //  1-> X  initialization purppses
              //  3-> ActualPicture --> From the beggining absolute picture counter
              //  4-> CurrentPicture --> 1-8 "Pack" picture counter   EE_ADD_CUR   
              // 30-> eeAddress Where to write next                   EE_ADD_ADD
              // 13-> ActualPicture --> Counter from begining         EE_ADD_PIC
              // 20-> camera current ISO (dongleless)                 EE_ADD_ISO

 


#endif
