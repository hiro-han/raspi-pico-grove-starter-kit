#ifndef PIN_ASSIGMENT_H_
#define PIN_ASSIGMENT_H_

namespace grove {

#define A0 26
#define A1 27
#define A2 28
#define D16 16
#define D18 18
#define D20 20
#define UART0_TX 1
#define UART0_RX 2
#define UART1_TX 4
#define UART1_RX 5
#define I2C0_SDA 8
#define I2C0_SCL 9
#define I2C1_SDA 6
#define I2C1_SCL 7

enum class ShieldPort {
  kA0,
  kA1,
  kA2,
  kD16,
  kD18,
  kD20,
  kUART0,
  kUART1,
  kI2C0,
  kI2C1
};

}  // namespace grove

#endif  // PIN_ASSIGMENT_H_