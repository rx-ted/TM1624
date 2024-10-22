#ifndef _TM1624_H_
#define _TM1624_H_

// !TM1624 driver select 7X11
/*
TM1624 Pinout:
             +----v----+
        DIO -|1      24|- GRD1
        CLK -|2      23|- GRD2
        STB -|3      22|- GND
        VDD -|4      21|- GRD3
       SEG1 -|5      20|- GRD4
       SEG2 -|6      19|- GND
       SEG3 -|7      18|- SEG14/GRD5
       SEG4 -|8      17|- SEG13/GRD6
       SEG5 -|9      16|- SEG12/GRD7
       SEG6 -|10     15|- SEG11
       SEG7 -|11     14|- SEG10
       SEG8 -|12     13|- SEG9
             +---------+
----------------------------------------------------------------
       SEG1 SEG2 SEG3 SEG4 SEG5 SEG6 SEG7 SEG8 SEG9 SEG10 SEG11
      |----|----|----|----|----|----|----|----|----|-----|-----|
GRID1 |    |    |    |    |    |    |    |    |    |     |     |
GRID2 |    |    |    |    |    |    |    |    |    |     |     |
GRID3 |    |    |    |    |    |    |    |    |    |     |     |
GRID4 |    |    |    |    |    |    |    |    |    |     |     |
GRID5 |    |    |    |    |    |    |    |    |    |     |     |
GRID6 |    |    |    |    |    |    |    |    |    |     |     |
GRID7 |    |    |    |    |    |    |    |    |    |     |     |
*/

#include "tm1624_font.h"

template <typename T, typename U>
auto min(T x, U y) -> decltype(x>y ? x : y)
{
  return x<y ? x : y;
}


#define TM1624_CMD_DATA_AUTO 0x40
#define TM1624_CMD_DATA_FIXED 0x44
#define TM1624_CMD_DISPLAY 0x80
#define TM1624_CMD_ADDRESS 0xC0
#define TM1624_MAX_POS 7
#define TM1624_MAX_SEGMENTS 11

typedef enum
{
    LIGHT1,
    LIGHT2,
    LIGHT3,
    LIGHT4,
    LIGHT5,
    LIGHT6,
    LIGHT7,
    LIGHT8,
} light_intensity_t;

typedef bool bit_t;

class TM1624
{

protected:
    uint16_t width, height;
    byte blocks;
    byte count_per_block;
    byte max_displays;
    byte max_segments;

private:
    uint32_t _dataPin;
    uint32_t _clockPin;
    uint32_t *_strobePins; // from left to right and from top to bottom.

    light_intensity_t _intensity;
    bit_t _activateDisplay;

    void _init();
    void _delay();
    void _start(byte block);
    void _stop(byte block);
    void _send(byte data);
    void _sendCommand(byte block, byte cmd);
    void _sendData(byte block, byte address, byte data);

public:
    TM1624(uint32_t dataPin, uint32_t clockPin,
           uint32_t *strobePins, byte strobeCols = 1,
           byte strobeRows = 1);

    uint16_t getHeight() { return this->height; }
    uint16_t getWidth() { return this->width; }
    byte getMaxSegment() { return this->max_segments; }
    byte getMaxDisplay() { return this->max_displays; }
    byte getBlocks() { return this->blocks; }

    /** Set the segments at a specific position on or off */
    void setupDisplay(bit_t activity, byte intensity, byte block = 0xff);
    void setSegments(uint16_t segment, byte block, byte position);
    void reverseBitOrder(uint16_t *segment);
};

#endif