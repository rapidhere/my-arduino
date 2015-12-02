#ifdef _BUILD_demo_rwifi

#include "rlib/framework.h"
#include "rlib/commondata.h"
#include "rlib/wifi/esp8266.h"

rapid::int8 flag = 0;
// rapid::Esp8266WifiBee wb(&Serial1, &Serial);
rapid::Esp8266WifiBee wb(&Serial1, NULL);

void setup() {
    pinMode(LED_PIN, OUTPUT);
    // Serial use as debug
    // Serial.begin(9600);

    // start wifi bee
    wb.begin();

    // work as work station
    // ap mode is not full supported yet
    if(wb.getMode() != wb.MODE_STATION) {
        wb.setMode(wb.MODE_STATION);
        wb.restart();
    }

    // timeout 20s
    if(! wb.connectAP(WIFI_SSID, WIFI_PASS, 20000))
        return;

    if(! wb.setMuxMode(wb.MODE_MUX_SINGLE))
        return ;

    if(wb.connect(wb.CONNECT_TYPE_TCP, SERVER_IP, SERVER_PORT_NUM) < 0)
        return ;

    flag = 1;
}

void loop() {
    if(flag) {
        rapid::blink(200);
    } else {
        rapid::blink(2000);
    }

    wb.send("GET /sensor/echo?message=1 HTTP/1.1\r\n"
        "\r\n");

    String ret;
    wb.checkMessage(&ret);
}

#endif
