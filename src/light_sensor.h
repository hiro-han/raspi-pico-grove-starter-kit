#ifndef LIGHT_SENSOR_H_
#define LIGHT_SENSOR_H_

#include <inttypes.h>

#include <string>

#include "hardware/gpio.h"
#include "pin_assignment.h"

namespace grove {
namespace light {

struct LightSensorResult {
  LightSensorResult() : brightness(0.0f), updated(false) {}
  float brightness;
  bool updated;
};

class LightSensor {
 public:
  LightSensor(const ShieldPort port);
  virtual ~LightSensor();

  LightSensorResult ReadBrightness();

 private:
  bool ReadGpio(int *data);
  int pin_;
  float conversion_factor_;
};

}  // namespace light
}  // namespace grove

#endif  // LIGHT_SENSOR_H_