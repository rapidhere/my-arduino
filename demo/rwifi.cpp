#ifdef _BUILD_demo_rwifi

#include "rlib/framework.h"
#include "rlib/commondata.h"
#include "rlib/wifi/esp8266.h"

rapid::int8 flag = 0;
rapid::Esp8266WifiBee wb(&Serial1, &Serial);

void setup() {
    pinMode(LED_PIN, OUTPUT);
    // Serial use as debug
    Serial.begin(9600);

    // start wifi bee
    wb.begin();

    delay(2000);

    if(wb.getMode() != wb.MODE_STATION) {
        wb.setMode(wb.MODE_STATION);
        wb.restart();
    }

    wb.debug("start connect");
    // timeout 20s
    if(wb.connectAP(WIFI_SSID, WIFI_PASS, 20000)) {
        flag = 1;
    }
}

void loop() {
    if(flag) {
        rapid::blink(200);
    } else {
        rapid::blink(2000);
    }
}

#endif
