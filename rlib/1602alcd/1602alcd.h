/**
 * A simple driver to display and controll 1602a lcd
 *
 * just a simple driver to work with 8-width mode 1602a lcd, auto-cursor screen
*/


#ifndef _R_1602ALCD_H
#define _R_1602ALCD_H 1

#include "types.h"
#include "Arduino.h"

#define LCD1602A_N_DATA_PINS 8

#define LCD1602A_RS_COMMAND LOW
#define LCD1602A_RS_DATA HIGH

#define LCD1602A_RW_WRITE LOW
#define LCD1602A_RW_READ HIGH

// cmd flags
// clear screen
#define LCD1602A_CLR 0x01
// set display
#define LCD1602A_FS 0x20
// choose 5x7 or 5x10 block
#define LCD1602A_FS_F57 0x04
#define LCD1602A_FS_F5A 0x00
// 2 line or 1 line
#define LCD1602A_FS_N2 0x08
#define LCD1602A_FS_N1 0x00
// 8-width or 4-width
#define LCD1602A_FS_DL8 0x10
#define LCD1602A_FS_DL4 0x00

// set AC mode
#define LCD1602A_AM 0x04
// AC Direction
#define LCD1602A_AM_ACL 0x00
#define LCD1602A_AM_ACR 0x02
// display move entry or not
#define LCD1602A_AM_SM 0x01
#define LCD1602A_AM_SN 0x00

// set display on/off control
#define LCD1602A_DS 0x08
// turn on/off display
#define LCD1602A_DS_ON 0x04
#define LCD1602A_DS_OF 0x00
// cursor on/off
#define LCD1602A_DS_CRON 0x02
#define LCD1602A_DS_CROFF 0x00
// cursor blink
#define LCD1602A_DS_CRBON 0x01
#define LCD1602A_DS_CRBOFF 0x00

// the pin info struct
struct Lcd1602aT {
    int8 rs; // the port of data/cmd pin
    int8 rw; // the port of read/write pin
    int8 e;  // the port of e pin
    int8 d[LCD1602A_N_DATA_PINS]; // data pins
};

void lcd1602a_initialize(struct Lcd1602aT *l);

void lcd1602a_write_command(struct Lcd1602aT *l, int command);
void lcd1602a_write_data(struct Lcd1602aT *l, int data);

#endif
