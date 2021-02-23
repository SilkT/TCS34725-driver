#include "app_tcs34725.h"

static uint16_t r, g, b, w, lux, par;

TaskHandle_t m_tcs34725_thread;

void tcs34725_thread(void * arg)
{
  UNUSED_PARAMETER(arg);
  while(1)
  {
    tcs34725_getRawData(&r, &g, &b, &w);
    lux = tcs34725_calculateLux(r, g, b);
    par = tcs34725_calculatePAR(w, r, g, b);
    NRF_LOG_INFO("R: %u", r);
    NRF_LOG_INFO("G: %u", g);
    NRF_LOG_INFO("B: %u", b);
    NRF_LOG_INFO("W: %u", w);
    NRF_LOG_INFO("Lux: %u", lux);
    NRF_LOG_INFO("PAR: %u", par);
    vTaskDelay(1000);
  }
}