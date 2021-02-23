#include "rtos_taskConfig.h"

SemaphoreHandle_t twiSemaphore = NULL;

void mutexInit(void *pvParameters, SemaphoreHandle_t *semaphore_handle)
{
    /* Create a mutex type semaphore. */
    *semaphore_handle = xSemaphoreCreateMutex();

    if (semaphore_handle != NULL)
    {
        __NOP();
        /* The semaphore was created successfully and
       can be used. */
    }
}