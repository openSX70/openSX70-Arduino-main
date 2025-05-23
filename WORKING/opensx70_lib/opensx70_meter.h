#ifndef OPENSX70_METER_H
#	error "inlude meter.h instead of this file"
#endif

#ifndef OPENSX70_METER_CHIP_H
  #	define OPENSX70_METER_CHIP_H
#else
  #	error "several meter chip set. Only one should be set. See settings.h"
#endif

#ifndef OPENSX70_INTEGRATOR_H
  #define OPENSX70_INTEGRATOR_H
  /* These functions are called by the main loop.
   * They provide uniform handling for whatever light sensor is used on a specific board.
   * Note : over/under-exposure is handled by the wheel on the camera at left hand.
   * So it is transparent for the program.
   */
  
  // for memory, generic functions :
  /*
  void meter_init();
  void meter_set_iso();
  void meter_compute();
  void meter_integrate();
  bool meter_update();
  */
  
  // Init timer 1 for reading sensor, enable sensor
  void integrator_init();

  void integrator_reset();

  // Start to integrate measure.
  void meter_led(byte _selector, byte _type);
#endif
