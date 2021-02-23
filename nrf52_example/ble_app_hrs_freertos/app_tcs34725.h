#include "../TCS34725-driver/drv_tcs34725.h"
#include "FreeRTOS.h"
#include "task.h"


extern TaskHandle_t m_tcs34725_thread;
void tcs34725_thread(void * arg);