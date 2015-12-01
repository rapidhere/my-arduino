#ifndef _R_ESP8266_H
#define _R_ESP8266_H 1

#include "Arduino.h"
#include "rlib/types.h"

namespace rapid {

class Esp8266WifiBee {
public:
    Esp8266WifiBee(HardwareSerial* wifiSerial, Stream* debug);

    // set the serial port to work
    // NOTE: typically you do not have to change the baudrate, beacause ESP8266 module require this baudrate
    void begin(uint32 baudrate=115200);

    // auto detect wifi encrypt mode
    // NOTE: haven't escape, ssid and pwd cannot contain double-quote
    bool connectAP(String ssid, String pwd, uint32 timeout_ms);

    // set to work under softap mode or station mode
    // you can set the mode before start the esp
    // or after set, you must restart the esp
    void setMode(uint8 mode);
    uint8 getMode();

    // restart esp
    void restart();

    // write debug info to serial
    void debug(String info);

    // wifi work mode
    // same number as doc specified
    static const uint8
    MODE_SOFT_AP,
    MODE_STATION,
    MODE_BOTH;
protected:
    // read up line, include end \r\n
    String readLine();

    void write(String cmd);

private:
    HardwareSerial* wifiSerial;
    Stream* debugSerial;
};

} // namespace rapid

#endif
