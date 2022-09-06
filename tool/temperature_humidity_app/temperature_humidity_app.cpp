#include <stdio.h>
#include <unistd.h>

#include <string>

#include "dht11.h"
#include "lcd_1602.h"
#include "pico/stdlib.h"
#include "utils.h"

int main() {
  stdio_init_all();

  grove::lcd::Lcd1602 lcd(grove::ShieldPort::kI2C1);
  lcd.Initialize();

  grove::dht::Dht11 th(grove::ShieldPort::kD16);
  grove::dht::DhtResult dht_result;

  sleep_us(1000 * 1000);

  lcd.Clear();
  while (true) {
    dht_result = th.ReadTempAndHumidity();
    if (dht_result.updated) {
      printf("{\"Temp\":\"%3.1f\",\"Hum\":\"%3.1f\"}", dht_result.temperature,
             dht_result.humidity);

      lcd.SetCursor(0, 0);
      lcd.Print(format("Temp : %3.1f", dht_result.temperature));
      lcd.SetCursor(0, 1);
      lcd.Print(format("Humi : %3.1f", dht_result.humidity));
    }
    sleep_us(1 * 1000 * 1000);
  }
  return 0;
}
