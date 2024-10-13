#include "tm1624.h"
#include <stdint.h>

TM1624::TM1624(byte dataPin, byte clockPin, byte strobePin, byte numDigits, bool activateDisplay, byte intensity) : TM16xx(dataPin, clockPin, strobePin, TM1624_MAX_POS, numDigits, activateDisplay, intensity)
{
    // Display mode command setting: 11 segments 7 bits
    _maxSegments = 11;
    sendCommand(0x03);
    clearDisplay();
    setupDisplay(activateDisplay, intensity);
}

void TM1624::clearDisplay()
{
    memset(_data, 0, sizeof(_data)); // clear it to 0.
    setDisplay();
}

void TM1624::setSegments(byte segments, byte position)
{
    if (position < _maxDisplays * 2)
        _data[(byte)position / 2] |= position % 2 == 0 ? segments : segments << 8;
}

void TM1624::setSegments16(uint16_t segments, byte position)
{
    if (position < _maxDisplays)
        _data[position] |= segments;
}

void TM1624::setDisplay()
{
    for (byte i = 0; i < _maxDisplays; i++)
    {
        sendData(i << 1, _data[i] & 0xff);
        sendData((i << 1) | 1, _data[i] >> 8);
    }
}

void TM1624::setPixel(byte x, byte y)
{
    if (x < _maxSegments && y < _maxDisplays)
        _data[y] |= 1 << x;
}

void TM1624::clearPixel(byte x, byte y)
{
    if (x < _maxSegments && y < _maxDisplays)
        _data[y] &= ~(1 << x);
}

bool TM1624::getPixel(byte x, byte y)
{
    return (_data[y] >> x) & 1;
}

void TM1624::sendNumber(byte x, byte y, uint8_t num)
{
    // The font size is 5 rows and 3 cols.
    for (byte i = y, j = 0; i < y + 5; i++, j++)
    {
        if (i > _maxDisplays)
            continue;
        byte dots = TM1624_NUMBER_FONT_3X5[num][j];
        // The design of segments pins is in descending order and can generally be annotated.
        {
            byte b1 = ((dots >> 0) & 1) << 2;
            byte b2 = ((dots >> 1) & 1) << 1;
            byte b3 = ((dots >> 2) & 1) << 0;
            dots = b3 | b2 | b1;
        }
        _data[i] |= dots << x;
    }
}

void TM1624::sendDot(byte x, byte y)
{
    for (byte i = y, j = 0; i < y + 5; i++, j++)
    {
        if (i > _maxDisplays)
            continue;
        byte dots = TM1624_NUMBER_FONT_DOT[j];
        _data[i] |= dots << x;
    }
}
