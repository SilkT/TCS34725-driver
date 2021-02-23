#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "nrf_log.h"
#include "timers.h"

void timers_init(void);
void application_timers_start(void);

#define BASE_TIMER_INTERVAL         100                                    /**< Battery level measurement interval (ms). */
#define OSTIMER_WAIT_FOR_QUEUE              2                                       /**< Number of ticks to wait for the timer queue to be ready */

