#ifndef logging_h
    #define logging_h
    #include "Arduino.h"
    /*
    class OpenSX70_Serial{
        public:
            
    };*/
    extern void serial_init();
    extern void output_line_serial(String const& input);
    extern void output_serial(String const& input);
#endif