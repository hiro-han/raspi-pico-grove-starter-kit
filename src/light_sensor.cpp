#include "light_sensor.h"

#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "pin_assignment.h"

namespace grove {
namespace light {

LightSensor::LightSensor(const ShieldPort port) {
  if (port == ShieldPort::kA0) {
    pin_ = A0;
  } else if (port == ShieldPort::kA1) {
    pin_ = A1;
  } else if (port == ShieldPort::kA2) {
    pin_ = A2;
  } else {
    pin_ = A0;
  }

  conversion_factor_ = 3.3f / (1 << 12);

  adc_init();
  adc_gpio_init(pin_);
  adc_select_input(0);
}

LightSensor::~LightSensor() {}

LightSensorResult LightSensor::ReadBrightness() {
  uint16_t read_value = adc_read();

  LightSensorResult result;
  result.updated = true;
  result.brightness = static_cast<float>(read_value) * conversion_factor_;

  return result;
}

}  // namespace light
}  // namespace grove