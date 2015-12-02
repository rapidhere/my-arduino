#include "rlib/wifi/esp8266.h"

namespace rapid {

const uint8 Esp8266WifiBee::MODE_STATION = 1;
const uint8 Esp8266WifiBee::MODE_SOFT_AP = 2;
const uint8 Esp8266WifiBee::MODE_BOTH = 3;

const uint8 Esp8266WifiBee::MODE_MUX_SINGLE = 0;
const uint8 Esp8266WifiBee::MODE_MUX_MULTIPLE = 1;

const uint8 Esp8266WifiBee::OK = 0;
const uint8 Esp8266WifiBee::FAIL = -1;
const uint8 Esp8266WifiBee::ALREADY_CONNECT = -2;
const uint8 Esp8266WifiBee::CLOSED = -3;
const uint8 Esp8266WifiBee::TIMEOUT = -4;

const String Esp8266WifiBee::CONNECT_TYPE_TCP = "TCP";
const String Esp8266WifiBee::CONNECT_TYPE_UDP = "UDP";

#define _DEBUG(info) \
    if(debugSerial) \
        debug(info)

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

uint8 Esp8266WifiBee::getMuxMode() {
    write("AT+CIPMUX?\r\n");
    delay(200);
    String buff;
    uint8 mode_no = -1;

    // read up to +CIPMUX:?
    while(true) {
        buff = readLine();
        if(buff[0] == '+')
            mode_no = buff[buff.length() - 3] - '0';
        else if(buff == "OK\r\n")
            break;
    }

    return mode_no;
}

bool Esp8266WifiBee::setMuxMode(uint8 mode) {
    switch(mode) {
    case MODE_MUX_SINGLE:
        write("AT+CIPMUX=0\r\n");
        break;
    case MODE_MUX_MULTIPLE:
        write("AT+CIPMUX=1\r\n");
        break;
    default:
        return false;
    }

    while(true) {
        String buf = readLine();
        if(buf == "OK\r\n")
            return true;

        if(buf == "Link is builded\r\n")
            return false;
    }
}

uint8 Esp8266WifiBee::connect(String connectType, String addr, uint16 port) {
    return _connect(MODE_MUX_SINGLE, -1, connectType, addr, port);
}

uint8 Esp8266WifiBee::connect(uint8 id, String connectType, String addr, uint16 port) {
    return _connect(MODE_MUX_MULTIPLE, id, connectType, addr, port);
}

uint8 Esp8266WifiBee::_connect(uint8 muxType, uint8 id, String connectType, String addr, uint16 port) {
    String cmd = "AT+CIPSTART=";

    if(muxType == MODE_MUX_MULTIPLE) {
        cmd += id;
    }

    cmd += "\"";
    cmd += connectType + "\",\"" + addr + "\"," + port + "\r\n";

    write(cmd);
    delay(200);

    while(true) {
        String buff = readLine();

        if(buff == "OK\r\n")
            return OK;
        else if(buff == "ERROR\r\n")
            return FAIL;
        else if(buff == "ALREADY CONNECT\r\n")
            return ALREADY_CONNECT;
    }
}

bool Esp8266WifiBee::closeAllConnection() {
    return _closeConnection(MODE_MUX_MULTIPLE, 5);
}

bool Esp8266WifiBee::closeConnection() {
    return _closeConnection(MODE_MUX_SINGLE, -1);
}

bool Esp8266WifiBee::closeConnection(uint8 id) {
    return _closeConnection(MODE_MUX_MULTIPLE, id);
}

bool Esp8266WifiBee::_closeConnection(uint8 muxType, uint8 id) {
    String cmd = "AT+CIPCLOSE";
    if(muxType == MODE_MUX_MULTIPLE) {
        cmd += "=";
        cmd += id;
    }
    cmd += "\r\n";

    write(cmd);
    delay(200);

    while(true) {
        String buff = readLine();

        if(buff == "OK\r\n")
            return true;
        else if(buff == "ERROR\r\n")
            return false;
        else if(buff == "link is not\r\n")
            return false;
    }
}

bool Esp8266WifiBee::send(String data) {
    return _send(MODE_MUX_SINGLE, -1, data);
}

bool Esp8266WifiBee::send(uint8 id, String data) {
    return _send(MODE_MUX_MULTIPLE, id, data);
}

bool Esp8266WifiBee::_send(uint8 muxType, uint8 id, String data) {
    String cmd = "AT+CIPSEND=";

    if(muxType == MODE_MUX_MULTIPLE) {
        cmd += id;
        cmd += ",";
    }

    cmd += data.length();
    cmd += "\r\n";

    write(cmd);
    delay(20);
    write(data.c_str());

    while(true) {
        String buff = readLine();

        if(buff == "SEND OK\r\n")
            return true;
        else if(buff == "ERROR\r\n")
            return false;
    }
}

uint8 Esp8266WifiBee::checkMessage(String *ret) {
    return _checkMessage(MODE_MUX_SINGLE, NULL, ret);
}

uint8 Esp8266WifiBee::checkMessage(uint8 *id, String *ret) {
    return _checkMessage(MODE_MUX_MULTIPLE, id, ret);
}

String Esp8266WifiBee::readData(uint32 timeout_ms) {
    uint32 start = millis();
    String buff;
    // read all data
    while(millis() - start < timeout_ms) {
        if(wifiSerial->available()) {
            buff += (char)wifiSerial->read();
        }
    }

    return buff;
}

uint8 Esp8266WifiBee::_checkMessage(uint8 muxType, uint8 *id, String *ret) {
    String buff = readData(100);

    if(buff.substring(2, 6) == "+IPD") {
        uint32 len;
        if(muxType == MODE_MUX_MULTIPLE) {
            if(id)
                *id = buff[7] - '0';
            len = buff.substring(9, buff.indexOf(':')).toInt();
        } else {
            len = buff.substring(7, buff.indexOf(':')).toInt();
        }

        String result = buff.substring(buff.indexOf(':') + 1);
        while(result.length() < len) {
            result += readData(20);
        }

        result = result.substring(0, len);
        if(*ret)
            (*ret) = result;
        return OK;
    } else if(buff.indexOf("CLOSED") > 0) {
        if(muxType == MODE_MUX_MULTIPLE && id) {
            *id = buff[buff.indexOf(',') - 1] - '0';
        }

        return CLOSED;
    }

    return TIMEOUT;
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
        if(wifiSerial->available()) {
            String ret = readLine();

            if(ret == "OK\r\n")
                return true;
            else if(ret == "FAIL\r\n")
                return false;
        }
    }

    _DEBUG("timeout");
    return false;
}

inline void Esp8266WifiBee::write(String cmd) {
    wifiSerial->write(cmd.c_str());
    wifiSerial->flush();
}

} // namespace rapid

#undef _DEBUG
