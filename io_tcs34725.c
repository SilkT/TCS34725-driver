#include "io_tcs34725.h"
#include "drv_tcs34725.h"

#include "nrf_drv_twi.h"
#include "nrf_delay.h"

/* TWI instance ID. */
#define TWI_INSTANCE_ID 0

/* TWI instance. */
extern const nrf_drv_twi_t m_twi;

io_tcs34725_t io_tcs34725 =
    {
        ._write = tcs34725_io_write, ._read = tcs34725_io_read, ._delay = tcs34725_io_delay, .semaphore_mutex_handle = NULL};

void tcs34725_io_set_mutex(SemaphoreHandle_t *semaphore_mutex_handle)
{
    io_tcs34725.semaphore_mutex_handle = semaphore_mutex_handle;
}

void tcs34725_io_write(uint8_t reg, uint8_t value)
{
    uint8_t data_to_write[2];
    data_to_write[0] = TCS34725_COMMAND_BIT | reg;
    data_to_write[1] = value & 0xFF;
    //implement writing function for your platform here
    ret_code_t err_code;
#ifdef IO_USE_RTOS_MUTEX
    if (io_tcs34725.semaphore_mutex_handle != NULL)
    {
        if (xSemaphoreTake(*io_tcs34725.semaphore_mutex_handle, TWI_MUTEX_TIMEOUT_TICKS))
        {
            err_code = nrf_drv_twi_tx(&m_twi, TCS34725_ADDRESS, data_to_write, sizeof(uint8_t)*2, true);
            xSemaphoreGive(*io_tcs34725.semaphore_mutex_handle);
            if(err_code!=0)APP_ERROR_HANDLER(err_code);
        }
    }
#else
    err_code = nrf_drv_twi_tx(&m_twi, TCS34725_ADDRESS, &reg_to_write, sizeof(uint8_t), true);
    err_code = nrf_drv_twi_rx(&m_twi, TCS34725_ADDRESS, rvalue, sizeof(uint8_t) * len);
#endif
}
void tcs34725_io_read(uint8_t reg, uint8_t *rvalue, uint8_t len)
{
    uint8_t reg_to_write;
    reg_to_write = TCS34725_COMMAND_BIT | reg;
    //implement receiving function for your platform here
    ret_code_t err_code;
#ifdef IO_USE_RTOS_MUTEX
    if (io_tcs34725.semaphore_mutex_handle != NULL)
    {
        if (xSemaphoreTake(*io_tcs34725.semaphore_mutex_handle, TWI_MUTEX_TIMEOUT_TICKS))
        {
            err_code = nrf_drv_twi_tx(&m_twi, TCS34725_ADDRESS, &reg_to_write, sizeof(uint8_t), true);
            err_code = nrf_drv_twi_rx(&m_twi, TCS34725_ADDRESS, rvalue, sizeof(uint8_t) * len);
            xSemaphoreGive(*io_tcs34725.semaphore_mutex_handle);
            if(err_code!=0)APP_ERROR_HANDLER(err_code);
        }
    }
#else
    err_code = nrf_drv_twi_tx(&m_twi, TCS34725_ADDRESS, &reg_to_write, sizeof(uint8_t), true);
    err_code = nrf_drv_twi_rx(&m_twi, TCS34725_ADDRESS, rvalue, sizeof(uint8_t) * len);
#endif
}

void tcs34725_io_delay(uint32_t milliseconds)
{
    // implement delay function for your platform here
    nrf_delay_ms(milliseconds);
}