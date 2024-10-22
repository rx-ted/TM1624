#include "tm1624.h"
#include <stdint.h>

TM1624::TM1624(uint32_t dataPin,
               uint32_t clockPin,
               uint32_t *strobePins,
               byte strobeCols,
               byte strobeRows) : _dataPin(dataPin),
                                  _clockPin(clockPin),
                                  _strobePins(strobePins),
                                  max_displays(TM1624_MAX_POS),
                                  max_segments(TM1624_MAX_SEGMENTS),
                                  _activateDisplay(true),
                                  _intensity(LIGHT4)
{
    blocks = strobeCols * strobeRows;
    width = strobeCols * max_segments;
    height = strobeRows * max_displays;
    count_per_block = width * height / blocks;

    _init();
};

void TM1624::_delay()
{
#if F_CPU > 100000000
    delayMicroseconds(1);
#endif
}

void TM1624::_init()
{
    pinMode(_dataPin, OUTPUT);
    pinMode(_clockPin, OUTPUT);
    digitalWrite(_clockPin, HIGH);
    for (uint8_t i = 0; i < blocks; i++)
    {
        pinMode(_strobePins[i], OUTPUT);
        digitalWrite(_strobePins[i], HIGH);
        // Display mode command setting: 11 segments 7 bits
        _sendCommand(0x03, i);
    }
    setupDisplay(_activateDisplay, _intensity);
}

void TM1624::_start(byte block)
{
    if (block < blocks)
    {
        digitalWrite(_strobePins[block], LOW);
        _delay();
    }
}
void TM1624::_stop(byte block)
{
    if (block < blocks)
    {
        digitalWrite(_strobePins[block], HIGH);
        _delay();
    }
}

void TM1624::_send(byte data)
{
    for (int i = 0; i < 8; i++)
    {
        digitalWrite(_clockPin, LOW);
        _delay();
        digitalWrite(_dataPin, data & 1 ? HIGH : LOW);
        _delay();
        data >>= 1;
        digitalWrite(_clockPin, HIGH);
        _delay();
    }
    _delay();
}

void TM1624::_sendCommand(byte block, byte cmd)
{
    _start(block);
    _send(cmd);
    _stop(block);
}

void TM1624::_sendData(byte block, byte address, byte data)
{
    _sendCommand(block, TM1624_CMD_DATA_FIXED); // use fixed addressing for data
    _start(block);
    _send(TM1624_CMD_ADDRESS | address); // address command + address
    _send(data);
    _stop(block);
}

void TM1624::setupDisplay(bit_t activity, byte intensity, byte block)
{
    byte cmd = TM1624_CMD_DISPLAY | (activity ? 8 : 0) | min(7, intensity);
    if (block == 0xff)
    {
        for (byte i = 0; i < blocks; i++)
            _sendCommand(i, cmd);
    }
    else
        _sendCommand(block, cmd);
}

void TM1624::reverseBitOrder(uint16_t *value)
{
    uint16_t segment = 0;
    for (byte i = 0; i < max_segments; i++)
    {
        segment |= (((*value) >> i) & 1) << (max_segments - i - 1);
    }
    value = &segment;
}

void TM1624::setSegments(uint16_t segment, byte block, byte position)
{
    reverseBitOrder(&segment);
    _sendData(block, position << 1, segment & 0xff);
    _sendData(block, (position << 1) | 1, segment >> 8 & 0b111);
}