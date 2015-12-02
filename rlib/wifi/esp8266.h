#ifndef _R_ESP8266_H
#define _R_ESP8266_H 1

#include "Arduino.h"
#include "rlib/types.h"

/**
 * just a simple driver to run esp8266 wifi
 *
 * do not use this in production envrioument
 * BUGGY: driver will block if some unexpected behaviour of the board happens, i haven't implement a timeout-mechiniasm
 */
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
    uint8 getMode();
    void setMode(uint8 mode);

    // set mux mode
    // only valid when all connect closed
    // if you started a ap-server, you must restart and then you can set mux mode
    // return true on success
    uint8 getMuxMode();
    bool setMuxMode(uint8 mode);

    // connect as tcp/udp
    // use under single_mux mode
    uint8 connect(String connectType, String addr, uint16 port);
    bool closeConnection();
    bool send(String data);
    uint8 checkMessage(String* ret);
    // use under multiple_mux mode
    uint8 connect(uint8 id, String connectType, String addr, uint16 port);
    bool closeConnection(uint8 id);
    bool closeAllConnection();
    bool send(uint8 id, String data);
    uint8 checkMessage(uint8* id, String* ret);
    // use general
    uint8 _connect(uint8 muxType, uint8 id, String connectType, String addr, uint16 port);
    bool _closeConnection(uint8 muxType, uint8 id);
    bool _send(uint8 muxType, uint8 id, String data);
    uint8 _checkMessage(uint8 muxType, uint8* id, String* ret);

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

    // mux mode
    // same number as doc specified
    static const uint8
    MODE_MUX_SINGLE,
    MODE_MUX_MULTIPLE;

    // connect type
    static const String
    CONNECT_TYPE_TCP,
    CONNECT_TYPE_UDP;

    // connect result
    static const uint8
    OK,
    FAIL,
    ALREADY_CONNECT,
    CLOSED,
    TIMEOUT;
protected:
    // read up line, include end \r\n
    String readLine();
    
    String readData(uint32 timeout_ms);

    void write(String cmd);

private:
    HardwareSerial* wifiSerial;
    Stream* debugSerial;
};

} // namespace rapid

#endif
