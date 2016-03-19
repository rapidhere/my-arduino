#include "dht22.h"
#include "Arduino.h"
#include "rlib/framework.h"

namespace rapid {

DHT22::DHT22(uint8 _pin) : pin(_pin), temperature(0), humidity(0), nextMilli(0) {
}

uint8 DHT22::setup() {
    pinMode(pin, OUTPUT);
    pinHigh();

    return 1;
}

uint8 DHT22::loadData() {
    // wait for at least 2000ms
    if(millis() < nextMilli) {
        return 0;
    }
    nextMilli = millis() + MIN_WAIT_MS;

    noInterrupts();

    // mention sensor to load data
    pinMode(pin, OUTPUT);
    pinLow();
    delay(10);
    pinHigh();
    delayMicroseconds(40);

    // clear data
    data[0] = data[1] = data[2] = data[3] = data[4] = 0;

    // ready to read data
    pinMode(pin, INPUT);
    delayMicroseconds(160);

    for(int32 i = 0;i < DATA_BIT_LEN;i ++) {
        int8 counter = 0, bit;
        // wait for gap
        while(pinRead() == LOW) {
          delayMicroseconds(COUNTER_DELAY_US);
        }

        if(counter >= 200)
            break;

        // read bit
        counter = 0;
        while(pinRead() == HIGH) {
            counter ++;
            delayMicroseconds(COUNTER_DELAY_US);
        }

        if(counter >= 200)
            break;

        // get bit and store
        bit = (counter >= MIN_TRUE_COUNTER ? 1 : 0);
        data[i >> 3] |= (bit << (i & 0x3));
    }

    interrupts();

    pinMode(pin, OUTPUT);

    // then parse data
    // checksum check
    if(data[4] != (data[0] + data[1] + data[2] + data[3]) & 0xff) {
        return 0;
    }

    humidity = (data[0] << 8 | data[1]);

    temperature = (((data[2] & 0x7f) << 8) | data[3]);
    if(data[2] & 0x80) temperature = -temperature;

    return 1;
}

int32 DHT22::getTemperature() {
    return temperature;
}

int32 DHT22::getHumidity() {
    return humidity;
}

void DHT22::pinHigh() {
    digitalWrite(pin, HIGH);
}

void DHT22::pinLow() {
    digitalWrite(pin, LOW);
}

uint8 DHT22::pinRead() {
    return digitalRead(pin);
}

}
