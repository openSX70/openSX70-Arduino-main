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

  void meter_led(byte _selector, byte _type);
#endif
