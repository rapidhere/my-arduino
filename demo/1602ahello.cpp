#ifdef _BUILD_demo_1602ahello

#include "rlib/framework.h"
#include "rlib/1602alcd/1602alcd.h"

struct rapid::Lcd1602aT lcd;

const char* content = "Wild Way!Rap:D!";

void setup() {
    lcd.rs = 14;
    lcd.rw = 15;
    lcd.e = 16;
    lcd.d[0] = 20;
    lcd.d[1] = 21;
    lcd.d[2] = 22;
    lcd.d[3] = 23;
    lcd.d[4] = 24;
    lcd.d[5] = 25;
    lcd.d[6] = 26;
    lcd.d[7] = 27;
    rapid::lcd1602a_initialize(&lcd);

    const char* ch;
    for(ch = content;*ch;ch ++)
        rapid::lcd1602a_write_data(&lcd, *ch);
}

void loop() {
}

#endif
