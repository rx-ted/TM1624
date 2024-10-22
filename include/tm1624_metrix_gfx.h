#ifndef _TM1624_METRIX_GFX_H_
#define _TM1624_METRIX_GFX_H_

#include "tm1624.h"
#include "Adafruit_GFX.h"

class TM1624MatrixGFX : public GFXcanvas8
{
private:
    void _write(byte block);

protected:
    bit_t *buffer;
    uint16_t buffer_size;
    TM1624 *_module;

public:
    TM1624MatrixGFX(TM1624 *module);
    void updateBuffer();
    void write();

    void setIntensity(light_intensity_t intensity, bit_t activity = true, byte block = 0xff);
};

#endif