#ifndef TMPERATRUE_HUMIDITY_H_
#define TMPERATRUE_HUMIDITY_H_

#include <inttypes.h>

#include <string>

#include "hardware/gpio.h"
#include "pin_assignment.h"

namespace grove {
namespace dht {

const uint MAX_TIMINGS = 85;
const int COUNT_TH = 30;

struct DhtResult {
  DhtResult() : humidity(0.0f), temperature(0.0f), updated(false) {}
  float humidity;
  float temperature;
  bool updated;
};

class Dht11 {
 public:
  Dht11(const ShieldPort port);
  virtual ~Dht11();

  DhtResult ReadTempAndHumidity();

 private:
  bool ReadGpio(int *data);
  int pin_;
};

}  // namespace dht
}  // namespace grove

#endif  // TMPERATRUE_HUMIDITY_H_