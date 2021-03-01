#include "app_tcs34725.h"

static uint16_t r, g, b, w, lux, par;
tcs34725Flicker_t flick_w;
bool isFlicking_w;

TaskHandle_t m_tcs34725_thread;

void tcs34725_thread(void * arg)
{
  UNUSED_PARAMETER(arg);
  tcs34725_setIntegrationTime(TCS34725_INTEGRATIONTIME_2_4MS);
  mutexInit(NULL, &twiSemaphore);
  tcs34725_io_set_mutex(&twiSemaphore);  
  if(!tcs34725_begin()){
    NRF_LOG_INFO("tcs34725 init failed");
  } else NRF_LOG_INFO("tcs34725 init succes");
  
  tcs34725_flickerInit(&flick_w, 20, 3);
  while(1)
  {
    tcs34725_getRawData(&r, &g, &b, &w);
    lux = tcs34725_calculateLuxf(r, g, b);
    par = tcs34725_calculatePAR(w, r, g, b);
    isFlicking_w = tcs34725_checkFlicker(&flick_w, w);
    NRF_LOG_INFO("Raw W: %u", w);
    NRF_LOG_INFO("Raw R: %u", r);
    NRF_LOG_INFO("Raw G: %u", g);
    NRF_LOG_INFO("Raw B: %u", b);
    NRF_LOG_INFO("PAR: " NRF_LOG_FLOAT_MARKER, NRF_LOG_FLOAT(par));
    NRF_LOG_INFO("LUX: " NRF_LOG_FLOAT_MARKER, NRF_LOG_FLOAT(lux));
    NRF_LOG_INFO("White flicking: %u", isFlicking_w);
    vTaskDelay(500);
  }
}