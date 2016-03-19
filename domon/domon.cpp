#ifdef _BUILD_domon

#include "rlib/framework.h"
#include "rlib/commondata.h"
#include "rlib/wifi/esp8266.h"
#include "rlib/dht22/dht22.h"

#undef WIFI_PASS
#undef WIFI_SSID

/*data config*/
#define WIFI_SSID "rapid-test"
#define WIFI_PASS "12345678"

#define TEMP_SENSOR_DATA_PIN 14

/*setup status flag*/
rapid::uint8 setupFlag = 1;

/*wifi bee driver*/
rapid::Esp8266WifiBee wb(&Serial1, NULL);

/*th driver*/
rapid::DHT22 dht(TEMP_SENSOR_DATA_PIN);

rapid::uint8 setupWifi();
rapid::uint8 setupSensor();

void sendData(rapid::int32 temperature);

void setup() {
    // wait for devices
    delay(1000);

    pinMode(LED_PIN, OUTPUT);

    if(! setupWifi()) {
      setupFlag = 0;
      return;
    }

    if(! setupSensor()) {
      setupFlag = 0;
      return ;
    }
}

void loop() {
    // check setup
    if(!setupFlag) {
      rapid::blink(200);
      return;
    }

    /*working blink*/
    rapid::blink(1000);

    dht.loadData();
}

void sendData(rapid::int32 temperature) {
    String msg = "GET /sensor/echo?message=";
    msg += temperature;
    msg += " HTTP/1.1\r\n\r\n";

    wb.send(msg);
}

rapid::uint8 setupSensor() {
  if(! dht.setup()) {
      return 0;
  }
  return 1;
}

rapid::uint8 setupWifi() {
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
      return 0;

  if(! wb.setMuxMode(wb.MODE_MUX_SINGLE))
      return 0;

  if(wb.connect(wb.CONNECT_TYPE_TCP, SERVER_IP, SERVER_PORT_NUM) < 0)
      return 0;

  return 1;
}

#endif
