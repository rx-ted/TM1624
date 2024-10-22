#include "tm1624_metrix_gfx.h"

TM1624MatrixGFX::TM1624MatrixGFX(TM1624 *module) : GFXcanvas8(module->getWidth(), module->getHeight())
{
    _module = module;
    buffer_size = _module->getHeight() * _module->getWidth();
    buffer = (bit_t *)malloc(buffer_size);
    fillScreen(0);
}

void TM1624MatrixGFX::_write(byte block)
{
    byte currentRow = (byte)block / (_module->getBlocks() / _module->getMaxSegment());
    byte currentCol = (byte)block % (_module->getBlocks() / _module->getMaxSegment());

    for (byte y = currentRow * _module->getMaxDisplay(); y < currentRow * _module->getMaxDisplay() + _module->getMaxDisplay(); y++)
    {
        uint16_t segment = 0;
        for (byte x = currentCol * _module->getMaxSegment(); x < currentCol * _module->getMaxSegment() + _module->getMaxSegment(); x++)
            segment |= buffer[y * WIDTH + x] << (x % _module->getMaxSegment());
        _module->setSegments(segment, block, y % _module->getMaxDisplay());
    }
}

void TM1624MatrixGFX::updateBuffer()
{
    uint8_t *buf = getBuffer();
    for (uint16_t i = 0; i < buffer_size; i++)
    {
        buffer[i] = buf[i] == 0 ? 0 : 1;
    }
}

void TM1624MatrixGFX::write()
{
    updateBuffer();
    byte blocks = _module->getHeight() * _module->getWidth() / (_module->getMaxDisplay() * _module->getMaxSegment());
    for (byte block = 0; block < blocks; block++)
    {
        _write(block);
    }
}

void TM1624MatrixGFX::setIntensity(light_intensity_t intensity, bit_t activity, byte block)
{
    _module->setupDisplay(activity, intensity, block);
}