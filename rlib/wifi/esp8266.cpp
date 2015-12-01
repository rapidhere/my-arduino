#include "rlib/wifi/esp8266.h"

namespace rapid {

const uint8 Esp8266WifiBee::MODE_STATION = 1;
const uint8 Esp8266WifiBee::MODE_SOFT_AP = 2;
const uint8 Esp8266WifiBee::MODE_BOTH = 3;

void Esp8266WifiBee::debug(String info) {
    if(debugSerial)
        debugSerial->println(info);
}

Esp8266WifiBee::Esp8266WifiBee(HardwareSerial* _s, Stream* _d) {
    wifiSerial = _s;
    debugSerial = _d;
}

void Esp8266WifiBee::begin(uint32 baudrate) {
    wifiSerial->begin(baudrate);
    while(!(*wifiSerial));

    debug("serial start");
}

void Esp8266WifiBee::setMode(uint8 mode) {
    switch(mode) {
    case MODE_SOFT_AP:
        write("AT+CWMODE=2\r\n");
        break;
    case MODE_STATION:
        write("AT+CWMODE=1\r\n");
        break;
    case MODE_BOTH:
        write("AT+CWMODE=3\r\n");
        break;
    }
    delay(200);
    // read up ok
    while(readLine() != "OK\r\n");

    return;
}

String Esp8266WifiBee::readLine() {
    String ret;
    char data;
    uint8 len = 0;

    while(true) {
        if(wifiSerial->available()) {
            data = wifiSerial->read();

            if(data > 0) {
                len ++;
                ret += data;

                if(len >= 2 && ret[len - 2] == '\r' && ret[len - 1] == '\n')
                    break;
            }
        }
    }

    debug(String("# getline [") + ret.length() + "]");
    debug(ret);

    return ret;
}

uint8 Esp8266WifiBee::getMode() {
    write("AT+CWMODE?\r\n");
    delay(200);
    String buff;
    uint8 mode_no = -1;

    // read up `+CWMODE:?`
    while(true) {
        buff = readLine();
        if(buff[0] == '+')
            mode_no = buff[buff.length() - 3] - '0';
        else if(buff == "OK\r\n")
            break;
    }

    return mode_no;
}

void Esp8266WifiBee::restart() {
    write("AT+RST\r\n");
    delay(200);
    // read up possible data
    while(readLine() != "ready\r\n");
}

bool Esp8266WifiBee::connectAP(String ssid, String pwd, uint32 timeout_ms) {
    write("AT+CWJAP=\"" + ssid + "\",\"" + pwd + "\"\r\n");
    delay(300);

    uint32 start = millis();

    while(millis() - start < timeout_ms) {
        String ret = readLine();

        if(ret == "OK\r\n")
            return true;
        else if(ret == "FAIL\r\n")
            return false;
    }

    debug("timeout");
    return false;
}

inline void Esp8266WifiBee::write(String cmd) {
    wifiSerial->write(cmd.c_str());
    wifiSerial->flush();
}

} // namespace rapid
