#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t m_logger_thread;                                /**< Definition of Logger thread. */

void log_init(void);
void logger_thread(void *arg);