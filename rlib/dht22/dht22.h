#ifndef _R_DHT22_H
#define _R_DHT22_H 1

#include "rlib/types.h"

#define DATA_BIT_LEN 40
#define COUNTER_DELAY_US 3
#define MIN_TRUE_COUNTER 10
#define MIN_WAIT_MS 2000

namespace rapid {

class DHT22 {
public:
    DHT22(uint8 _pin);
    uint8 setup();
    uint8 loadData();

    int32 getTemperature();
    int32 getHumidity();
//private:
    uint8 pin;
    uint8 data[6];
    uint32 nextMilli;
    int32 temperature, humidity;

    void pinHigh();
    void pinLow();
    uint8 pinRead();
};

}

#endif
