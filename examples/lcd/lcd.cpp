#include <stdio.h>
#include <unistd.h>

#include <string>

#include "lcd_1602.h"
#include "pico/stdlib.h"

int main() {
  grove::lcd::Lcd1602 lcd(grove::ShieldPort::kI2C1);

  lcd.Initialize();
  lcd.Print("hello, world!");

  sleep_us(1000 * 1000);

  lcd.Clear();
  lcd.Print("Count :");

  int count = 0;
  while (true) {
    sleep_us(1 * 1000 * 1000);
    lcd.SetCursor(2, 1);
    lcd.Print(std::to_string(count));

    count++;
  }
  return 0;
}
