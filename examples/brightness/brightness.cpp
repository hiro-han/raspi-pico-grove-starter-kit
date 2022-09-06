#include <stdio.h>
#include <unistd.h>

#include <iomanip>
#include <sstream>
#include <string>

#include "lcd_1602.h"
#include "light_sensor.h"
#include "pico/stdlib.h"
#include "utils.h"

int main() {
  stdio_init_all();

  grove::lcd::Lcd1602 lcd(grove::ShieldPort::kI2C1);
  lcd.Initialize();

  grove::light::LightSensor light_sensor(grove::ShieldPort::kA0);
  grove::light::LightSensorResult light_result;

  sleep_us(1000 * 1000);

  lcd.Clear();
  while (true) {
    light_result = light_sensor.ReadBrightness();
    if (light_result.updated) {
      lcd.SetCursor(0, 0);
      lcd.Print(format("Bright : %3.3f", light_result.brightness));
    }
    sleep_us(1 * 1000 * 1000);
  }
  return 0;
}
