#include <arduino.h>
#include "cbusdefs.h"
#include "CBUScolourMap.h"
// 

void CBUScolourMap(uint16_t* colMap)
{
  uint8_t opcode = 0;
  int i;
  uint16_t * p = colMap;
  for (i= 0; i < COLOUR_MAP_SIZE; i++)
  {
    uint16_t colour = COLR_BLACK;   // Default colour is black
    switch (opcode) 
    {
      // General messages
      case OPC_ACK:	// General ack
      case OPC_NAK:	// General nak
      case OPC_HLT:	// Bus Halt
      case OPC_BON:	// Bus on
      case OPC_DBG1:	// Debug message with 1 status byte
      case OPC_EXTC:	// Extended opcode
      case OPC_EXTC1:	// Extended opcode with 1 data byte
      case OPC_EXTC2:	// Extended opcode with 2 data bytes
      case OPC_EXTC3:	// Extended opcode with 3 data bytes
      case OPC_EXTC4:	// Extended opcode with 4 data bytes
      case OPC_EXTC5:	// Extended opcode with 5 data bytes
      case OPC_EXTC6:	// Extended opcode with 6 data byes
      colour = CBUS_GENERAL_COLR;
      break;
      //
      // Traction messages
      case OPC_TOF:	// Track off
      case OPC_TON:	// Track on
      case OPC_ESTOP:	// Track stopped
      case OPC_ARST:	// System reset
      case OPC_RTOF:	// Request track off
      case OPC_RTON:	// Request track on
      case OPC_RESTP:	// Request emergency stop all 
      case OPC_KLOC:	// Release engine by handle
      case OPC_QLOC:	// Query engine by handle
      case OPC_DKEEP:	// Keep alive for cab
      case OPC_RLOC:	// Request session for loco
      case OPC_QCON:	// Query consist
      case OPC_ALOC:	// Allocate loco (used to allocate to a shuttle in cancmd)
      case OPC_STMOD:	// Set Throttle mode
      case OPC_PCON:	// Consist loco
      case OPC_KCON:	// De-consist loco
      case OPC_DSPD:	// Loco speed/dir
      case OPC_DFLG:	// Set engine flags
      case OPC_DFNON:	// Loco function on
      case OPC_DFNOF:	// Loco function off
      case OPC_SSTAT:	// Service mode status
      case OPC_NNRSM:	// Reset to manufacturer's defaults
      case OPC_DFUN:	// Set engine functions
      case OPC_GLOC:	// Get loco (with support for steal/share)
      case OPC_ERR:	// Command station error
      case OPC_RDCC3:	// 3 byte DCC packet
      case OPC_WCVO:	// Write CV byte Ops mode by handle
      case OPC_WCVB:	// Write CV bit Ops mode by handle
      case OPC_QCVS:	// Read CV
      case OPC_PCVS:	// Report CV
      case OPC_RDCC4:	// 4 byte DCC packet
      case OPC_WCVS:	// Write CV service mode
      case OPC_RDCC5:	// 5 byte DCC packet
      case OPC_WCVOA:	// Write CV ops mode by address
      case OPC_CABDAT:	// Cab data (cab signalling)
      case OPC_RDCC6:	// 6 byte DCC packets
      case OPC_PLOC:	// Loco session report
      colour = CBUS_TRACTION_COLR;
      break;
 
      // Configuration messages
      case OPC_RSTAT:	// Request node status
      case OPC_QNN:	// Query nodes
      case OPC_RQNP:	// Read node parameters
      case OPC_RQMN:	// Request name of module type
      case OPC_SNN:	// Set node number 
      case OPC_RQNN:	// Request Node number in setup mode
      case OPC_NNREL:	// Node number release
      case OPC_NNACK:	// Node number acknowledge
      case OPC_NNLRN:	// Set learn mode
      case OPC_NNULN:	// Release learn mode
      case OPC_NNCLR:	// Clear all events
      case OPC_NNEVN:	// Read available event slots
      case OPC_NERD:	// Read all stored events
      case OPC_RQEVN:	// Read number of stored events
      case OPC_WRACK:	// Write acknowledge
      case OPC_RQDAT:	// Request node data event
      case OPC_RQDDS:	// Request short data frame
      case OPC_ENUM:	// Force can_id self enumeration
      case OPC_NNRST:	// Reset node (as in restart)
      case OPC_CMDERR:	// Errors from nodes during config
      case OPC_EVNLF:	// Event slots left response
      case OPC_NVRD:	// Request read of node variable
      case OPC_NENRD:	// Request read stored event by index
      case OPC_RQNPN:	// Request read module parameters
      case OPC_NUMEV:	// Number of events stored response
      case OPC_CANID:	// Set canid
      case OPC_EVULN:	// Unlearn event
      case OPC_NVSET:	// Set a node variable
      case OPC_NVANS:	// Node variable value response
      case OPC_PARAN:	// Single node parameter response
      case OPC_REVAL:	// Request read of event variable
      case OPC_EVLRN:	// Teach event
      case OPC_EVANS:	// Event variable read response in learn mode
      case OPC_NAME:	// Module name response
      case OPC_STAT:	// Command station status report
      case OPC_PARAMS:	// Node parameters response
      colour = CBUS_CONFIG_COLR;
      break;

      // Layout signalling
      case OPC_AREQ:	// Accessory Request event
      case OPC_ARON:	// Accessory response event on
      case OPC_AROF:	// Accessory response event off
      case OPC_ASRQ:	// Short Request event
      case OPC_ARSON:	// Accessory short response on event
      case OPC_ARSOF:	// Accessory short response off event
      case OPC_REQEV:	// Read event variable in learn mode
      case OPC_ARON1:	// Accessory on response (1 data byte)
      case OPC_AROF1:	// Accessory off response (1 data byte)
      case OPC_NEVAL:	// Event variable by index read response
      case OPC_PNN:	// Response to QNN
      case OPC_ARSON1:	// Short response event on with one data byte
      case OPC_ARSOF1:	// Short response event off with one data byte
      case OPC_FCLK:	// Fast clock
      case OPC_ARON2:	// Accessory on response
      case OPC_AROF2:	// Accessory off response
      case OPC_ARSON2:	// Short response event on with two data bytes
      case OPC_ARSOF2	:// Short response event off with two data bytes
      case OPC_ENRSP:	// Read node events response
      case OPC_ARON3:	// Accessory on response
      case OPC_AROF3:	// Accessory off response
      case OPC_EVLRNI:	// Teach event using event indexing
      case OPC_ACDAT: // Accessory data event: 5 bytes of node data (eg: RFID)
      case OPC_ARDAT:	// Accessory data response
      case OPC_DDES:	// Short data frame aka device data event (device id plus 5 data bytes)
      case OPC_DDRS:	// Short data frame response aka device data response
      case OPC_DDWS:	// Device Data Write Short
      case OPC_ARSON3:	// Short response event on with 3 data bytes
      case OPC_ARSOF3:	// Short response event off with 3 data bytes
      colour = CBUS_LAYOUT_COLR;
      break;


      case OPC_ACON:	// on event
      case OPC_ASON:	// Short event on 
      case OPC_ACON1:	// On event with one data byte
      case OPC_ASON1:	// Accessory short on with 1 data byte
      case OPC_ACON2:	// On event with two data bytes
      case OPC_ASON2:	// Accessory short on with 2 data bytes
      case OPC_ACON3:	// On event with 3 data bytes
      case OPC_ASON3:	// Accessory short on with 3 data bytes
      colour = CBUS_LAYOUT_ON_COLR;
      break;

      case OPC_ACOF:	// off event 
      case OPC_ASOF:	// Short event off
      case OPC_ACOF1:	// Off event with one data byte
      case OPC_ASOF1:	// Accessory short off with 1 data byte
      case OPC_ACOF2:	// Off event with two data bytes
      case OPC_ASOF2:	// Accessory short off with 2 data bytes
      case OPC_ACOF3:	// Off event with 3 data bytes
      case OPC_ASOF3:	// Accessory short off with 3 data bytes
      colour = CBUS_LAYOUT_OFF_COLR;
      break;

      // Software management
      case OPC_BOOT:	// Put node into boot mode
      colour = CBUS_SW_COLR;
      break;

      default:
         Serial.println ("Unknown OPC");
         colour = CBUS_DEFAULT_COLR;
         break;
    }
    *p = colour;
    opcode++;
    p++;
  }
}

