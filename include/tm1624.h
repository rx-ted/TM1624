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

#include "TM16xx.h"
#include "tm1624_font.h"

#define TM1624_MAX_POS 7

class TM1624 : public TM16xx
{
private:
    uint16_t _data[TM1624_MAX_POS];

public:
    TM1624(byte dataPin, byte clockPin, byte strobePin, byte numDigits = TM1624_MAX_POS, bool activateDisplay = true, byte intensity = 7);

    /** Set the segments at a specific position on or off */
    void clearDisplay();
    void setSegments(byte segments, byte position);
    void setSegments16(uint16_t segments, byte position);
    void setDisplay();
    void setPixel(byte nCol, byte nRow);
    void clearPixel(byte nCol, byte nRow);
    bool getPixel(byte nCol, byte nRow);
    void sendNumber(byte x, byte y, uint8_t num);
    void sendDot(byte x, byte y);
};

#endif // !_TM1624_H_