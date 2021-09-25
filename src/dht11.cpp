#include "dht11.h"

#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "pin_assignment.h"

namespace grove {
namespace dht {

Dht11::Dht11(const ShieldPort port) {
  if (port == ShieldPort::kD16) {
    pin_ = D16;
  } else if (port == ShieldPort::kD18) {
    pin_ = D18;
  } else if (port == ShieldPort::kD20) {
    pin_ = D20;
  } else {
    pin_ = D16;
  }

  gpio_init(pin_);
}

Dht11::~Dht11() {}

DhtResult Dht11::ReadTempAndHumidity() {
  int data[5] = {0, 0, 0, 0, 0};
  DhtResult result;

  if (ReadGpio(data)) {
    result.updated = true;
    result.humidity = data[0] + (float)data[1] * 0.1;
    result.temperature = data[2] + (float)(data[3] & 0x7F) * 0.1;
    if (data[3] & 0x80) {
      result.temperature = -result.temperature;
    }
  }
  return result;
}

bool Dht11::ReadGpio(int *data) {
  uint8_t last = 1;
  uint j = 0;

  gpio_set_dir(pin_, GPIO_OUT);
  gpio_put(pin_, 0);
  sleep_ms(18);
  gpio_put(pin_, 1);
  sleep_us(40);
  gpio_set_dir(pin_, GPIO_IN);

  for (uint i = 0; i < MAX_TIMINGS; i++) {
    uint count = 0;
    while (gpio_get(pin_) == last) {
      count++;
      sleep_us(1);
      if (count == 255) break;
    }
    last = gpio_get(pin_);
    if (count == 255) break;

    if ((i >= 4) && (i % 2 == 0)) {
      data[j / 8] <<= 1;
      if (count > COUNT_TH) data[j / 8] |= 1;
      j++;
    }
  }

  if ((j >= 40) &&
      (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))) {
    return true;
  }
  return false;
}

}  // namespace dht
}  // namespace grove