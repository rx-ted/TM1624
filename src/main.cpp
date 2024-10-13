#include "tm1624.h"
#include "TM16xx.h"

//  ~/.platformio/packages/framework-arduinoststm32/variants/STM32F1xx/F103C8T_F103CB(T-U)/variant_AFROFLIGHT_F103CB_XX.h

#define STB PB7
#define DIO PB8
#define CLK PB9

TM1624 module(DIO, CLK, STB);

uint8_t cnt = 0;

void setup()
{
}

void loop()
{

  delay(1000);
  module.setPixel(cnt, 0);
  module.sendNumber(1, 2, cnt++);
  module.sendDot(5, 2);
  module.sendNumber(7, 2, cnt++);
  module.setDisplay();
  delay(1000);

  module.clearPixel(cnt, 0);

  module.clearPixel(5, 5);
  module.clearPixel(5, 3);
  module.setDisplay();
  delay(1000);

  module.clearDisplay();
  if (cnt > 9)
  {
    cnt = 0;
  }
}