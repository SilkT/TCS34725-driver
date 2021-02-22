#ifndef _IO_TCS34725_H_
#define _IO_TCS34725_H_

#include <stdint.h>

typedef void (*writeReg)(uint8_t reg, uint8_t value);
typedef void (*readRegs)(uint8_t reg, uint8_t *rvalue, uint8_t len);
typedef void (*sysDelay)(uint32_t milliseconds);

typedef struct
{
    writeReg _write;
    readRegs _read;
    sysDelay _delay;
} io_tcs34725_t;
extern io_tcs34725_t io_tcs34725;

void tcs34725_io_write(uint8_t reg, uint8_t value);
void tcs32725_io_read(uint8_t reg, uint8_t *rvalue, uint8_t len);
void tcs32725_io_delay(uint32_t milliseconds);

#endif