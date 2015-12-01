#ifdef _BUILD_demo_wifi

#include "exlib/wifi/esp8266.h"
#include "rlib/framework.h"
#include "rlib/commondata.h"

rapid::int8 flag = 0;
Esp8266 wifi;

void setup() {
    delay(2000);

    pinMode(LED_PIN, OUTPUT);
    Serial1.begin(115200);
    wifi.begin(&Serial1);

    if(wifi.connectAP(WIFI_SSID, WIFI_PASS)) {
        flag = 1;
    } else {
        return ;
    }

    wifi.setSingleConnect();
    if(wifi.connectTCPServer(SERVER_IP, SERVER_PORT)) {
        flag = 1;
    } else {
        return ;
    }
}

void loop() {
    if(! flag) {
        rapid::blink(2000);
        return ;
    }
    rapid::blink(200);

    delay(1000);
    wifi.sendMessage("GET /sensor/echo?message=1 HTTP/1.1\r\n"
        "\r\n");
}

#endif
