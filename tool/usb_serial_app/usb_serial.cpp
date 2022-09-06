#include <stdio.h>
#include <unistd.h>

#include <iomanip>
#include <sstream>
#include <string>

#include "dht11.h"
#include "lcd_1602.h"
#include "light_sensor.h"
#include "pico/stdio_usb.h"
#include "pico/stdlib.h"
#include "utils.h"

void sendSerial(const float &temperature, const float &humidity,
                const float &brighness) {
  std::stringstream ss;
  ss << std::fixed << std::setprecision(3) << "{"
     << "\"temperature\": " << temperature << ","
     << "\"humidity\": " << humidity << ","
     << "\"brightness\": " << brighness << "}";

  printf(ss.str().c_str());
}

int main() {
  stdio_init_all();

  grove::lcd::Lcd1602 lcd(grove::ShieldPort::kI2C1);
  grove::dht::Dht11 th(grove::ShieldPort::kD16);
  grove::light::LightSensor light_sensor(grove::ShieldPort::kA0);

  grove::dht::DhtResult prev_dht_result;
  grove::light::LightSensorResult prev_light_result;

  lcd.Initialize();
  sleep_us(1000 * 1000);

  lcd.Clear();
  while (true) {
    grove::dht::DhtResult dht_result = th.ReadTempAndHumidity();
    if (dht_result.updated) {
      lcd.SetCursor(0, 0);
      lcd.Print(format("T:%3.1f H:%3.1f ", dht_result.temperature,
                       dht_result.humidity));
      prev_dht_result = dht_result;
    }

    grove::light::LightSensorResult light_result =
        light_sensor.ReadBrightness();
    if (light_result.updated) {
      lcd.SetCursor(0, 1);
      lcd.Print(format("B:%3.3f", light_result.brightness));
      prev_light_result = light_result;
    }

    sendSerial(dht_result.temperature, dht_result.humidity,
               light_result.brightness);

    sleep_us(1 * 1000 * 1000);
  }
  return 0;
}
