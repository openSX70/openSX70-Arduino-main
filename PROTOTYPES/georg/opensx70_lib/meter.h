/*
 * openSX70 : opensource project to take the SX70 instant camera beyond its original functionnalities
 * This file is part of openSX70 project.
 *
 *
 * This project is licensed under the terms of CreactiveCommons Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)
 *
 * You are free to :
 * Share — copy and redistribute the material in any medium or format
 *
 * Adapt — remix, transform, and build upon the material 
 * The licensor cannot revoke these freedoms as long as you follow the license terms.
 *
 *
 * Under the following terms :
 * Attribution — You must give appropriate credit, provide a link to the license,
 * and indicate if changes were made.
 * You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use. 
 *
 * NonCommercial — You may not use the material for commercial purposes.
 *
 * No additional restrictions — You may not apply legal terms or technological measures that legally restrict
 * others from doing anything the license permits.
 * 
 *
 * Notices:
 * You do not have to comply with the license for elements of the material in the public domain or
 * where your use is permitted by an applicable exception or limitation.
 *
 * No warranties are given. The license may not give you all of the permissions necessary for your intended use.
 * For example, other rights such as publicity, privacy, or moral rights may limit how you use the material.
 *
 * Complete license terms are available at : https://creativecommons.org/licenses/by-nc/4.0/legalcode
 */

#ifndef OPENSX70_METER_H
#define OPENSX70_METER_H

#include "settings.h"

// Meter deals with integrated lightmeter.
// These functions are to be defined for each specific meter chip that is used on the board.
// They will implement whatever is needed for this chip to provide the needed data.
// note : as they are hardware-specific implemented, there is no meter.cpp file.

// As we are in the developing process, their argument may evolve, as well as their return type.

// Self-explanatory : do whatever is needed for the chip to run on startup :
// Pin enable, one-wire / I2C / SPI interface initialisation
extern void meter_init();

// give the sensitivity to the meter
extern void meter_set_iso(uint16_t const& iso);

// compute the time needed for current ligth condition
// so it can be display as an indication in the viewfinder
extern int meter_compute(unsigned int _interval);

// start an integration cycle for auto exposure
extern void meter_integrate();

// get the update from the sensor. When enough light has reached the film, returns true.
extern bool meter_update();

extern void meter_set_iso(uint16_t const& iso);
//void meter_led(byte _selector, bool _type);

// preprocessor conditionnal inlusion for specific chip header file.
#if defined (TSL237T)
#	include "TSL237T.h"
#elif defined (TSL235R)
#	include "TSL235R.h"
#elif defined (TCS3200)
#	include "TCS3200.h"
#elif defined (TSL2591)
#	include "TSL2591.h"
#else
#	warning	"no meter included"
#endif

#endif
