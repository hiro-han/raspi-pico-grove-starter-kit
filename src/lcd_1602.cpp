#include "lcd_1602.h"

#include <stdio.h>
#include <string.h>

#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "pin_assignment.h"

namespace grove {
namespace lcd {

Lcd1602::Lcd1602(const I2C i2c) {
  uint32_t sda_pin;
  uint32_t scl_pin;
  if (i2c == I2C::kI2C0) {
    i2c_inst_ = i2c0;
    sda_pin = I2C0_SDA;
    scl_pin = I2C0_SCL;
  } else if (i2c == I2C::kI2C1) {
    i2c_inst_ = i2c1;
    sda_pin = I2C1_SDA;
    scl_pin = I2C1_SCL;
  }

  i2c_init(i2c_inst_, 100 * 1000);
  gpio_set_function(sda_pin, GPIO_FUNC_I2C);
  gpio_set_function(scl_pin, GPIO_FUNC_I2C);
  gpio_pull_up(sda_pin);
  gpio_pull_up(scl_pin);
  bi_decl(bi_2pins_with_func(sda_pin, scl_pin, GPIO_FUNC_I2C));
}

Lcd1602::~Lcd1602() {
  // nop
}

void Lcd1602::Initialize() {
  displayfunction_ |= LCD_2LINE;
  currline_ = 0;
  displayfunction_ |= LCD_5x8DOTS;
  sleep_us(50000);

  // Send function set command sequence
  Command(LCD_FUNCTIONSET | displayfunction_);
  sleep_us(50000);

  Command(LCD_FUNCTIONSET | displayfunction_);
  sleep_us(150);

  Command(LCD_FUNCTIONSET | displayfunction_);
  Command(LCD_FUNCTIONSET | displayfunction_);

  // turn the display on with no cursor or blinking default
  displaycontrol_ = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  Display();

  // clear it off
  Clear();

  // Initialize to default text direction (for romance languages)
  displaymode_ = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  Command(LCD_ENTRYMODESET | displaymode_);

  // backlight init
  SetReg(REG_MODE1, 0);
  // set LEDs controllable by both PWM and GRPPWM registers
  SetReg(REG_OUTPUT, 0xFF);
  // set MODE2 values
  // 0010 0000 -> 0x20  (DMBLNK to 1, ie blinky mode)
  SetReg(REG_MODE2, 0x20);
}

void Lcd1602::Clear() {
  Command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  sleep_us(2000);
}

void Lcd1602::Home() {
  Command(LCD_RETURNHOME);  // set cursor position to zero
  sleep_us(2000);
}

void Lcd1602::SetCursor(uint8_t col, uint8_t row) {
  unsigned char val = (row == 0 ? col | 0x80 : col | 0xc0);
  unsigned char dta[2] = {LCD_SETDDRAMADDR, val};
  SendByteS(dta, 2);
}

// Turn the display on/off (quickly)
void Lcd1602::NoDisplay() {
  displaycontrol_ &= ~LCD_DISPLAYON;
  Command(LCD_DISPLAYCONTROL | displaycontrol_);
}

void Lcd1602::Display() {
  displaycontrol_ |= LCD_DISPLAYON;
  Command(LCD_DISPLAYCONTROL | displaycontrol_);
}

void Lcd1602::NoCursor() {
  displaycontrol_ &= ~LCD_CURSORON;
  Command(LCD_DISPLAYCONTROL | displaycontrol_);
}

void Lcd1602::Cursor() {
  displaycontrol_ |= LCD_CURSORON;
  Command(LCD_DISPLAYCONTROL | displaycontrol_);
}

// Turn on and off the blinking cursor
void Lcd1602::NoBlink() {
  displaycontrol_ &= ~LCD_BLINKON;
  Command(LCD_DISPLAYCONTROL | displaycontrol_);
}

void Lcd1602::Blink() {
  displaycontrol_ |= LCD_BLINKON;
  Command(LCD_DISPLAYCONTROL | displaycontrol_);
}

// These commands scroll the display without changing the RAM
void Lcd1602::ScrollDisplayLeft(void) {
  Command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void Lcd1602::ScrollDisplayRight(void) {
  Command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void Lcd1602::LeftToRight(void) {
  displaymode_ |= LCD_ENTRYLEFT;
  Command(LCD_ENTRYMODESET | displaymode_);
}

// This is for text that flows Right to Left
void Lcd1602::RightToLeft(void) {
  displaymode_ &= ~LCD_ENTRYLEFT;
  Command(LCD_ENTRYMODESET | displaymode_);
}

// This will 'right justify' text from the cursor
void Lcd1602::Autoscroll(void) {
  displaymode_ |= LCD_ENTRYSHIFTINCREMENT;
  Command(LCD_ENTRYMODESET | displaymode_);
}

// This will 'left justify' text from the cursor
void Lcd1602::NoAutoscroll(void) {
  displaymode_ &= ~LCD_ENTRYSHIFTINCREMENT;
  Command(LCD_ENTRYMODESET | displaymode_);
}

void Lcd1602::CreateChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7;  // we only have 8 locations 0-7
  Command(LCD_SETCGRAMADDR | (location << 3));

  unsigned char dta[9];
  dta[0] = 0x40;
  for (int i = 0; i < 8; i++) {
    dta[i + 1] = charmap[i];
  }
  SendByteS(dta, 9);
}

// Control the backlight LED blinking
void Lcd1602::BlinkLED(void) {
  // blink period in seconds = (<reg 7> + 1) / 24
  // on/off ratio = <reg 6> / 256
  SetReg(0x07, 0x17);  // blink every second
  SetReg(0x06, 0x7f);  // half on, half off
}

void Lcd1602::NoBlinkLED(void) {
  SetReg(0x07, 0x00);
  SetReg(0x06, 0xff);
}

/*********** mid level commands, for sending data/cmds */

// send command
inline void Lcd1602::Command(uint8_t value) {
  unsigned char dta[2] = {LCD_SETDDRAMADDR, value};
  SendByteS(dta, 2);
}

// send data
inline size_t Lcd1602::Write(uint8_t value) {
  unsigned char dta[2] = {LCD_SETCGRAMADDR, value};
  SendByteS(dta, 2);
  return 1;  // assume sucess
}

void Lcd1602::SetReg(unsigned char addr, unsigned char dta) {
  i2c_write_blocking(i2c_inst_, RGB_ADDRESS, &addr, 1, false);
  i2c_write_blocking(i2c_inst_, RGB_ADDRESS, &dta, 1, false);
}

void Lcd1602::Print(const std::string& str) {
  int length = str.length() + 1;
  unsigned char dta[length];
  dta[0] = LCD_SETCGRAMADDR;
  memcpy(dta + 1, str.c_str(), length);
  SendByteS(dta, length);
}

void Lcd1602::SendByte(unsigned char dta) {
  i2c_write_blocking(i2c_inst_, LCD_ADDRESS, &dta, 1, false);
}

void Lcd1602::SendByteS(const unsigned char* dta, unsigned char len) {
  i2c_write_blocking(i2c_inst_, LCD_ADDRESS, dta, len, false);
}

}  // namespace lcd
}  // namespace grove