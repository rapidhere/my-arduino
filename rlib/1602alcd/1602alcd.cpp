#include "1602alcd.h"

namespace rapid {

static void pulse_enable(struct Lcd1602aT *l) {
    digitalWrite(l->e, HIGH);
    delay(1);
    digitalWrite(l->e, LOW);
}

void lcd1602a_initialize(struct Lcd1602aT *l) {
    int i;
    for(i = 0;i < LCD1602A_N_DATA_PINS;i ++)
        pinMode(l->d[i], OUTPUT);

    pinMode(l->e, OUTPUT);
    pinMode(l->rs, OUTPUT);
    pinMode(l->rw, OUTPUT);

    // wait
    delay(100);

    // set display
    // set to 8-bit, 2line, 5x7
    lcd1602a_write_command(l, LCD1602A_FS | LCD1602A_FS_F57 | LCD1602A_FS_N2 | LCD1602A_FS_DL8);
    delay(100);

    // set ac mode
    // set to left, no display
    lcd1602a_write_command(l, LCD1602A_AM | LCD1602A_AM_ACL | LCD1602A_AM_SN);
    delay(100);

    // set to no cursor and blink
    lcd1602a_write_command(l, LCD1602A_DS | LCD1602A_DS_ON | LCD1602A_DS_CROFF | LCD1602A_DS_CRBOFF);
    delay(100);

    // clear screen
    lcd1602a_write_command(l, LCD1602A_CLR);
    delay(100);
}

void lcd1602a_write_command(struct Lcd1602aT *l, int command) {
    int i, temp;

    digitalWrite(l->rs, LCD1602A_RS_COMMAND);
    digitalWrite(l->rw, LCD1602A_RW_WRITE);

    temp = command & 0xff;
    for(i = 0;i < LCD1602A_N_DATA_PINS;i ++) {
        digitalWrite(l->d[i], temp & 0x1);
        temp >>= 1;
    }

    pulse_enable(l);
    delay(37);
}

void lcd1602a_write_data(struct Lcd1602aT *l, int data) {
    int i, temp;

    digitalWrite(l->rs, LCD1602A_RS_DATA);
    digitalWrite(l->rw, LCD1602A_RW_WRITE);

    temp = data & 0xff;
    for(i = 0;i < LCD1602A_N_DATA_PINS;i ++) {
        digitalWrite(l->d[i], temp & 0x1);
        temp >>= 1;
    }
    pulse_enable(l);
    delay(37);
}

#undef LCD1602A_ENABLE

} // namespace rapid
