#ifndef _IO_TCS34725_H_
#define _IO_TCS34725_H_

#include    <stdint.h>
#include    "app_util_platform.h"

#define     IO_USE_RTOS_MUTEX
#ifdef      IO_USE_RTOS_MUTEX
#include    "../freeRTOS/rtos_taskConfig.h"
#include    "semphr.h"
#endif

typedef void (*writeReg)(uint8_t reg, uint8_t value);
typedef void (*readRegs)(uint8_t reg, uint8_t *rvalue, uint8_t len);
typedef void (*sysDelay)(uint32_t milliseconds);

typedef struct
{
    writeReg _write;
    readRegs _read;
    sysDelay _delay;
#ifdef      IO_USE_RTOS_MUTEX
    SemaphoreHandle_t * semaphore_mutex_handle;
#endif
} io_tcs34725_t;
extern io_tcs34725_t io_tcs34725;

#ifdef      IO_USE_RTOS_MUTEX
void tcs34725_io_set_mutex(SemaphoreHandle_t * semaphore_mutex_handle);
#endif
void tcs34725_io_write(uint8_t reg, uint8_t value);
void tcs34725_io_read(uint8_t reg, uint8_t *rvalue, uint8_t len);
void tcs34725_io_delay(uint32_t milliseconds);

#endif