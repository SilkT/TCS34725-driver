#include "../TCS34725-driver/drv_tcs34725.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "nrf_log.h"

#define TWI_MUTEX_TIMEOUT_TICKS   2

extern SemaphoreHandle_t twiSemaphore;

void mutexInit(void * pvParameters, SemaphoreHandle_t * semaphore_handle);