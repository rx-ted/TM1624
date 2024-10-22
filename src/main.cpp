
#include "Arduino.h"
#include "tm1624.h"
#include "tm1624_metrix_gfx.h"

#include "stm32f1xx.h"

//  ~/.platformio/packages/framework-arduinoststm32/variants/STM32F1xx/F103C8T_F103CB(T-U)/variant_AFROFLIGHT_F103CB_XX.h

#define DIO PB8
#define CLK PB9
uint32_t strobes[2] = {PB6, PB7};

TM1624 module(DIO, CLK, strobes, 2, 1);
TM1624MatrixGFX gfx(&module);

void Update_IT_callback(void)
{
  gfx.write(); // Update gfx buffer to display.
}

void setup()
{
  Serial.begin(115200);
  Serial.println("hello serial!");
  Serial.printf("STM32_CORE_VERSION: %d\n", STM32_CORE_VERSION);

  gfx.setIntensity(LIGHT1);

  TIM_TypeDef *Instance = TIM1;
  HardwareTimer *MyTim = new HardwareTimer(Instance);
  MyTim->setOverflow(10, HERTZ_FORMAT); // 10 Hz  => 100ms
  MyTim->attachInterrupt(Update_IT_callback);
  MyTim->resume();

  gfx.drawCircle(8, 3, 3, 1);
  gfx.drawRect(14, 0, 5, 6, 1);
  gfx.drawRoundRect(0, 0, 5, 5, 1, 1);
  delay(3000);
  gfx.fillScreen(0);
}

void loop()
{

  gfx.drawChar(0, 0, rand() % 0xff, 1, 0, 1);
  gfx.drawChar(6, 0, rand() % 0xff, 1, 0, 1);
  gfx.drawChar(11, 0, rand() % 0xff, 1, 0, 1);
  gfx.drawChar(17, 0, rand() % 0xff, 1, 0, 1);
  // gfx.write(); // interrupt timer, to define each 100 ms to run gfx.write()
  delay(1000); // delay 1000ms
}
