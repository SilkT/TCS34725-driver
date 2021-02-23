#include "rtos_timConfig.h"
#include "boards.h"

static TimerHandle_t m_base_timer;                               /**< Definition of battery timer. */

/**@brief   Function for starting application timers.
 * @details Timers are run after the scheduler has started.
 */
void application_timers_start(void)
{
    // Start application timers.
    if (pdPASS != xTimerStart(m_base_timer, OSTIMER_WAIT_FOR_QUEUE))
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
}

/**@brief Function for handling the Base timer time-out.
 *
 * @details This function will be called each time the battery level measurement timer expires.
 *
 * @param[in] xTimer Handler to the timer that called this function.
 *                   You may get identifier given to the function xTimerCreate using pvTimerGetTimerID.
 */
static void base_timer_timeout_handler(TimerHandle_t xTimer)
{
    bsp_board_led_invert(1);
    UNUSED_PARAMETER(xTimer);
}

/**@brief Function for the Timer initialization.
 *
 * @details Initializes the timer module. This creates and starts application timers.
 */
void timers_init(void)
{
    // Initialize timer module.
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

    // Create timers.
    m_base_timer = xTimerCreate("TIM",
                                   BASE_TIMER_INTERVAL,
                                   pdTRUE,
                                   NULL,
                                   base_timer_timeout_handler);

    /* Error checking */
    if (NULL == m_base_timer)
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
}
