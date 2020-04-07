/*
   openSX70 : opensource project to take the SX70 instant camera beyond its original functionnalities
   This file is part of openSX70 project.


   This project is licensed under the terms of CreactiveCommons Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)

   You are free to :
   Share — copy and redistribute the material in any medium or format

   Adapt — remix, transform, and build upon the material
   The licensor cannot revoke these freedoms as long as you follow the license terms.


   Under the following terms :
   Attribution — You must give appropriate credit, provide a link to the license,
   and indicate if changes were made.
   You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.

   NonCommercial — You may not use the material for commercial purposes.

   No additional restrictions — You may not apply legal terms or technological measures that legally restrict
   others from doing anything the license permits.


   Notices:
   You do not have to comply with the license for elements of the material in the public domain or
   where your use is permitted by an applicable exception or limitation.

   No warranties are given. The license may not give you all of the permissions necessary for your intended use.
   For example, other rights such as publicity, privacy, or moral rights may limit how you use the material.

   Complete license terms are available at : https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#ifndef OPENSX70_OPENSX70_H
#define OPENSX70_OPENSX70_H

// Don't change the order of the includes, as there are cross dependencies !

// All headers are included with this file, and each .cpp should include this one !
// There are exceptions, though : specific libraries for specific hardware on the camera
// or for the dongle are included in their specific files.

// includes for Arduino environnement
#include <Arduino.h>
//#include <EEPROM.h>



// includes for external libraries

#include "sx70_alpha_rw_pcb.h"
#include "settings.h"

#include "camera_functions.h"

//#include "ClickButton.h"


#include "uDongle2.h"

#include "ClickButton.h"

#include "eeprom_init.h"

#include "meter.h"

#endif
