#ifndef CBUScolourMap_h
#define CBUScolourMap_h

// #include <Arduino.h>  // for definition of byte datatype
#include "cbusdefs.h"

//
/// class to encapsulate colour assignment to CBUS messages
//
#define COLOUR_MAP_SIZE 256


// Map of CBUS opcodes to colours
// 
//
const uint16_t COLR_BLACK =    0;
const uint16_t COLR_BROWN =    0x1820;
const uint16_t COLR_RED =      0xF800;
const uint16_t COLR_DK_RED =   0x7800;
const uint16_t COLR_ORANGE =   0xFBE0;
const uint16_t COLR_YELLOW =   0xFFE0;
const uint16_t COLR_GREEN =    0x07E0;
const uint16_t COLR_DK_GREEN = 0x05E0;
const uint16_t COLR_CYAN =     0x07FF;
const uint16_t COLR_DK_CYAN =  0x03EF;
const uint16_t COLR_BLUE =     0x001F;
const uint16_t COLR_DK_BLUE =  0x0010;
const uint16_t COLR_MAGENTA =  0xF81F;
const uint16_t COLR_PURPLE =   0xE01C;
const uint16_t COLR_GREY =     0x4208;

const uint16_t CBUS_GENERAL_COLR =  COLR_BROWN;
const uint16_t CBUS_TRACTION_COLR = COLR_DK_CYAN;
const uint16_t CBUS_CONFIG_COLR =   COLR_PURPLE;
const uint16_t CBUS_LAYOUT_COLR =   COLR_DK_GREEN;
const uint16_t CBUS_LAYOUT_ON_COLR =       COLR_GREEN;
const uint16_t CBUS_LAYOUT_OFF_COLR =      COLR_RED;
const uint16_t CBUS_SW_COLR =       COLR_ORANGE;
const uint16_t CBUS_DEFAULT_COLR =  COLR_YELLOW;


extern void CBUScolourMap(uint16_t* colMap);

#endif
