// #include <Adafruit_CircuitPlayground.h>
// #include <Adafruit_Circuit_Playground.h>

// Ian Blair   M6893   MERG
// 
// Designed for Arduino Uno R4
//
// This sketch disokays CBUS activity on an SPI TFT display
// Connect Arduino 3.3v to OLED Vcc and CAN Tx/Rx
// Connect Arduino Gnd to OLED Gnd and CAN tx/Rx
//
// Arduino Data Pins
//(level shift to 3.3 V required for outputs... )
//
// D2  to DC
// D3  to CS
// D4  to CAN TX (CANTX0)
// D5  to CAN RX (CANRX0)
// 
// D8  to Display RST
// D11 to MOSI
// D12 to MISO
// D13 to SCK
// 

// In the Arduino IDE , Select Tools / Manage Libraries and upload the required Adafruit libraries
// indicated in the include statements below

#include <SPI.h> // 
#include <Arduino_CAN.h> //
#include <Adafruit_GFX.h> // Graphics and fonts library UPLOAD
#include <Adafruit_ILI9341.h>  // TFT LCD hardware drivers  UPLOAD
#include "cbusdefs.h"

#define SCREEN_WIDTH 360 // OLED display width, in pixels
#define SCREEN_HEIGHT 240 // OLED display height, in pixels

#define TFT_CS 3  // ou D10
#define TFT_DC 2  // Or D9?
#define TFT_RST 8 // D8
Adafruit_ILI9341 tftdisplay = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST); 
static uint32_t CanId = 1; // Temporary value

// This section run only once at startup
void display_setup() {
  // testing for correct display conguration
  // I2C OLED displays have either fixed 0x3C address or choice of 0x3C or 0x3D
  tftdisplay.begin();
  tftdisplay.fillScreen(ILI9341_WHITE);
  tftdisplay.setTextSize(1);
  tftdisplay.setTextColor(ILI9341_BLACK);
  tftdisplay.setCursor(0, 10);
  tftdisplay.println(" ILI9341 CBUS(r) Monitor v0.1");
  tftdisplay.println(" ");
}

// Display buffer
// Can message has 13 bytes. We allow 48 bytes when converted to hex (+count or timestamp?)
#define TFTDISPLAY_WIDTH 49   // Width in characters +1 for string terminator 
// We allow for 32 lines on display, each of any colout
#define TFTDISPLAY_LENGTH 32  // Number of lines (must be a power of two)
#define TFTDISPLAY_MASK (TFTDISPLAY_LENGTH-1)
typedef struct display_line {uint16_t text_colour; char text_data[TFTDISPLAY_WIDTH];};
display_line displayBuffer[TFTDISPLAY_LENGTH];
display_line * displayPtr = &displayBuffer[0];

void display_add(uint16_t text_colour, char * text_data)
{
  display_line* p = displayPtr;
  p->text_colour = text_colour;
  strcpy(p->text_data, text_data) ;
  p++;
  if ((p - &displayBuffer[0]) >= TFTDISPLAY_LENGTH) p = &displayBuffer[0];
  displayPtr = p;
}

void display_all()
{
  // Fill only require to completely blank display area
  //tftdisplay.fillRect(0,30, (5*TFTDISPLAY_WIDTH),(30+(7*TFTDISPLAY_LENGTH)),ILI9341_WHITE);
  tftdisplay.setCursor(0, 30);
  display_line* p = displayPtr;
  for (uint16_t i = 0; i < TFTDISPLAY_LENGTH; i++)  
  {
    // Use second (background) colour to avoid blanking
    tftdisplay.setTextColor(p->text_colour,ILI9341_WHITE);
    tftdisplay.println(p->text_data);
    p++;
    if ((p - &displayBuffer[0]) >= TFTDISPLAY_LENGTH) p = &displayBuffer[0];
  }   
}

// Allocates colour according to opcode category and value
uint16_t text_colour(uint8_t opcode)
{
  uint16_t colour;
// Categories are config, layout control, locomotive control and bus control
// layout On and off we allocate specific colours to (green for on, red for off)
  switch (opcode)
  {
  // Some Config messages
  case OPC_RQNN:
  case OPC_RQNP:
  case OPC_PARAMS:
  case OPC_NAME:
  case OPC_SNN:
  case OPC_NNACK:
    colour = ILI9341_MAGENTA;
    break;
  // Layout control ON
  case OPC_ACON:
  case OPC_ASON:
    colour = ILI9341_GREEN;
    break;
  // layout  control OFF
  case OPC_ACOF:
  case OPC_ASOF:
    colour = ILI9341_RED;
    break;
  // Traction messages
  case OPC_RDCC3:
  case OPC_RDCC4:
  case OPC_RDCC5:
    colour - ILI9341_BLUE;
  default:
    colour = ILI9341_BLACK;
    break;
  }
  return(colour);
}

void can_setup() {
  if (!CAN.begin(CanBitRate::BR_125k))
  {
    Serial.println(F("CAN.begin failed."));
    for (;;);
  }
}

// This section run only once at startup
void setup() {
  display_setup();
  can_setup();
}

static uint32_t msg_cnt = 0;

// This next section runs continuosly
void loop() 
{
  // Read serial port for commanded test functions
  if (Serial.available())
  {
    char c = Serial.read();
    // Only one implemented so far 
    // It a 't' is typed send a test message
    if (c == 't')
    {
      /* Assemble a CAN message with the format of
       * 0xCA 0xFE 0x00 0x00 [4 byte message counter]
       */
      // Use an RQNN for test purposes
      uint8_t const msg_data[] = {0x50,0,0};
      CanMsg const msg(CanStandardId(CanId), sizeof(msg_data), msg_data);

      /* Transmit the CAN message, capture and display an
       * error core in case of failure.
      */
      if (int const rc = CAN.write(msg); rc < 0)
      {
        Serial.print  ("CAN.write(...) failed with error code ");
        Serial.println(rc);
        // for (;;) { };
      }
    }
  }
 

// Flag to mark when display update required
  boolean update_display = false;
// Readback test
  while (CAN.available())
  {
    CanMsg const msg = CAN.read();
    // uint8_t msgCanId = msg.id;
    size_t msgLength = msg.data_length;
    // uint8_t thisTextData[msg.MAX_DATA_LENGTH];
    // Ignore any excess length
    if (msgLength > msg.MAX_DATA_LENGTH) msgLength = msg.MAX_DATA_LENGTH;
    char disp_data[TFTDISPLAY_WIDTH];
    char* disp_ptr = disp_data;
    // Note that Can ID has 32 bits
    disp_ptr += sprintf(disp_ptr, "%10d %03hx", msg_cnt, msg.id);    
    for(size_t i = 0; i < msgLength; i++)  
    {
      disp_ptr += sprintf(disp_ptr, " %02hx", msg.data[i]);
    }
    //*disp_ptr = '\0';
    Serial.println(disp_data);
    uint8_t opc = msg.data[0];
    uint16_t disp_colour = text_colour(opc);
    display_add(disp_colour, disp_data);
    update_display = true;
  }
  
  // you can change the delay for a longer or shorter clock cycle ( 5000 is approx 5 seconds )
  
  // char test_data[TFTDISPLAY_WIDTH];
  // sprintf(test_data, " %10d hhh oo dd dd dd dd dd dd dd", msg_cnt);
  // display_add(ILI9341_BLACK, test_data);
  // display_add(msg_cnt, test_data);
  if (update_display == true) display_all();
   /* Increase the loop counter. */
  msg_cnt++;
  // delay to be relooked at!
  // remove or set to 1?
  delay (1);
}
 
