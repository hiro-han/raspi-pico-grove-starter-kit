#ifndef LCD_H_
#define LCD_H_

#include <inttypes.h>

#include <string>

#include "hardware/i2c.h"
#include "pin_assignment.h"

namespace grove {
namespace lcd {

// Device I2C Arress
#define LCD_ADDRESS 0x3E
#define RGB_ADDRESS 0x62

// color define
#define WHITE 0
#define RED 1
#define GREEN 2
#define BLUE 3

#define REG_RED 0x04    // pwm2
#define REG_GREEN 0x03  // pwm1
#define REG_BLUE 0x02   // pwm0

#define REG_MODE1 0x00
#define REG_MODE2 0x01
#define REG_OUTPUT 0x08

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

#define MAX_LINES 2
#define MAX_CHARS 16

class Lcd1602 {
 public:
  Lcd1602(const ShieldPort i2c);
  virtual ~Lcd1602();

  void Initialize();
  void Clear();
  void Home();
  void NoDisplay();
  void Display();
  void NoBlink();
  void Blink();
  void NoCursor();
  void Cursor();
  void ScrollDisplayLeft();
  void ScrollDisplayRight();
  void LeftToRight();
  void RightToLeft();
  void Autoscroll();
  void NoAutoscroll();
  void CreateChar(uint8_t location, uint8_t charmap[]);
  void SetCursor(uint8_t, uint8_t);
  virtual size_t Write(uint8_t);
  void Command(uint8_t);
  void BlinkLED(void);
  void NoBlinkLED(void);
  void Print(const std::string& str);

 private:
  void SetReg(unsigned char addr, unsigned char dta);
  void SendByte(unsigned char dta);
  void SendByteS(const unsigned char* dta, unsigned char len);
  uint8_t displayfunction_;
  uint8_t displaycontrol_;
  uint8_t displaymode_;
  uint8_t initialized_;
  i2c_inst_t* i2c_inst_;

  uint8_t currline_;
};

}  // namespace lcd
}  // namespace grove

#endif  // LCD_H_