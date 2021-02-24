#include <math.h>
#include "drv_tcs34725.h"
#include "io_tcs34725.h"

tcs34725_t tcs34725 = {
    ._isInitialised = false,
    ._integrationTime = TCS34725_INTEGRATIONTIME_2_4MS,
    ._gain = TCS34725_GAIN_1X};

void tcs34725_enable()
{
  io_tcs34725._write(TCS34725_ENABLE, TCS34725_ENABLE_PON);
  io_tcs34725._delay(3);
  io_tcs34725._write(TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);
  /* Set a io_tcs34725._delay for the integration time.
    This is only necessary in the case where enabling and then
    immediately trying to read values back. This is because setting
    AEN triggers an automatic integration, so if a read RGBC is
    performed too quickly, the data is not yet valid and all 0's are
    returned */
  switch (tcs34725._integrationTime)
  {
  case TCS34725_INTEGRATIONTIME_2_4MS:
    io_tcs34725._delay(3);
    break;
  case TCS34725_INTEGRATIONTIME_24MS:
    io_tcs34725._delay(24);
    break;
  case TCS34725_INTEGRATIONTIME_50MS:
    io_tcs34725._delay(50);
    break;
  case TCS34725_INTEGRATIONTIME_101MS:
    io_tcs34725._delay(101);
    break;
  case TCS34725_INTEGRATIONTIME_154MS:
    io_tcs34725._delay(154);
    break;
  case TCS34725_INTEGRATIONTIME_700MS:
    io_tcs34725._delay(700);
    break;
  }
}

/*!
 *  @brief  Disables the device (putting it in lower power sleep mode)
 */
void tcs34725_disable()
{
  /* Turn the device off to save power */
  uint8_t reg = 0;
  io_tcs34725._read(TCS34725_ENABLE, &reg, 1);
  io_tcs34725._write(TCS34725_ENABLE, reg & ~(TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN));
}

/*!
 *  @brief  Initializes I2C and configures the sensor
 *  @param  addr
 *          i2c address
 *  @return True if initialization was successful, otherwise false.
 */

/*!
 *  @brief  Initializes I2C and configures the sensor
 *  @return True if initialization was successful, otherwise false.
 */
bool tcs34725_begin()
{
  tcs34725._i2caddr = TCS34725_ADDRESS;
  return tcs34725_init();
}

/*!
 *  @brief  Part of begin
 *  @return True if initialization was successful, otherwise false.
 */
bool tcs34725_init()
{
  /* Make sure we're actually connected */
  uint8_t x;
  io_tcs34725._read(TCS34725_ID, &x, 1);
  if ((x != 0x44) && (x != 0x10))
  {
    return false;
  }
  tcs34725._isInitialised = true;

  /* Set default integration time and gain */
  tcs34725_setIntegrationTime(tcs34725._integrationTime);
  tcs34725_setGain(tcs34725._gain);

  /* Note: by default, the device is in power down mode on bootup */
  tcs34725_enable();

  return true;
}

/*!
 *  @brief  Sets the integration time for the TC34725
 *  @param  it
 *          Integration Time
 */
void tcs34725_setIntegrationTime(tcs34725IntegrationTime_t it)
{
  if (!tcs34725._isInitialised)
    tcs34725_begin();

  /* Update the timing register */
  io_tcs34725._write(TCS34725_ATIME, it);

  /* Update value placeholders */
  tcs34725._integrationTime = it;
}

/*!
 *  @brief  Adjusts the gain on the TCS34725
 *  @param  gain
 *          Gain (sensitivity to light)
 */
void tcs34725_setGain(tcs34725Gain_t gain)
{
  if (!tcs34725._isInitialised)
    tcs34725_begin();

  /* Update the timing register */
  io_tcs34725._write(TCS34725_CONTROL, gain);

  /* Update value placeholders */
  tcs34725._gain = gain;
}

/*!
 *  @brief  Reads the raw red, green, blue and clear channel values
 *  @param  *r
 *          Red value
 *  @param  *g
 *          Green value
 *  @param  *b
 *          Blue value
 *  @param  *c
 *          Clear channel value
 */
void tcs34725_getRawData(uint16_t *r, uint16_t *g, uint16_t *b,
                         uint16_t *c)
{
  if (!tcs34725._isInitialised)
    tcs34725_begin();

  io_tcs34725._read(TCS34725_CDATAL, (uint8_t *)c, 2);
  io_tcs34725._read(TCS34725_RDATAL, (uint8_t *)r, 2);
  io_tcs34725._read(TCS34725_GDATAL, (uint8_t *)g, 2);
  io_tcs34725._read(TCS34725_BDATAL, (uint8_t *)b, 2);

  /* Set a io_tcs34725._delay for the integration time */
  switch (tcs34725._integrationTime)
  {
  case TCS34725_INTEGRATIONTIME_2_4MS:
    io_tcs34725._delay(3);
    break;
  case TCS34725_INTEGRATIONTIME_24MS:
    io_tcs34725._delay(24);
    break;
  case TCS34725_INTEGRATIONTIME_50MS:
    io_tcs34725._delay(50);
    break;
  case TCS34725_INTEGRATIONTIME_101MS:
    io_tcs34725._delay(101);
    break;
  case TCS34725_INTEGRATIONTIME_154MS:
    io_tcs34725._delay(154);
    break;
  case TCS34725_INTEGRATIONTIME_700MS:
    io_tcs34725._delay(700);
    break;
  }
}

/*!
 *  @brief  Reads the raw red, green, blue and clear channel values in
 *          one-shot mode (e.g., wakes from sleep, takes measurement, enters
 *          sleep)
 *  @param  *r
 *          Red value
 *  @param  *g
 *          Green value
 *  @param  *b
 *          Blue value
 *  @param  *c
 *          Clear channel value
 */
void tcs34725_getRawDataOneShot(uint16_t *r, uint16_t *g, uint16_t *b,
                                uint16_t *c)
{
  if (!tcs34725._isInitialised)
    tcs34725_begin();

  tcs34725_enable();
  tcs34725_getRawData(r, g, b, c);
  tcs34725_disable();
}

/*!
 *  @brief  Read the RGB color detected by the sensor.
 *  @param  *r
 *          Red value normalized to 0-255
 *  @param  *g
 *          Green value normalized to 0-255
 *  @param  *b
 *          Blue value normalized to 0-255
 */
void tcs34725_getRGB(float *r, float *g, float *b)
{
  uint16_t red, green, blue, clear;
  tcs34725_getRawData(&red, &green, &blue, &clear);
  uint32_t sum = clear;

  // Avoid divide by zero errors ... if clear = 0 return black
  if (clear == 0)
  {
    *r = *g = *b = 0;
    return;
  }

  *r = (float)red / sum * 255.0;
  *g = (float)green / sum * 255.0;
  *b = (float)blue / sum * 255.0;
}

/*!
 *  @brief  Converts the raw R/G/B values to color temperature in degrees Kelvin
 *  @param  r
 *          Red value
 *  @param  g
 *          Green value
 *  @param  b
 *          Blue value
 *  @return Color temperature in degrees Kelvin
 */
uint16_t tcs34725_calculateColorTemperature(uint16_t r, uint16_t g,
                                            uint16_t b)
{
  float X, Y, Z; /* RGB to XYZ correlation      */
  float xc, yc;  /* Chromaticity co-ordinates   */
  float n;       /* McCamy's formula            */
  float cct;

  if (r == 0 && g == 0 && b == 0)
  {
    return 0;
  }

  /* 1. Map RGB values to their XYZ counterparts.    */
  /* Based on 6500K fluorescent, 3000K fluorescent   */
  /* and 60W incandescent values for a wide range.   */
  /* Note: Y = Illuminance or lux                    */
  X = (-0.14282F * r) + (1.54924F * g) + (-0.95641F * b);
  Y = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);
  Z = (-0.68202F * r) + (0.77073F * g) + (0.56332F * b);

  /* 2. Calculate the chromaticity co-ordinates      */
  xc = (X) / (X + Y + Z);
  yc = (Y) / (X + Y + Z);

  /* 3. Use McCamy's formula to determine the CCT    */
  n = (xc - 0.3320F) / (0.1858F - yc);

  /* Calculate the final CCT */
  cct =
      (449.0F * powf(n, 3)) + (3525.0F * powf(n, 2)) + (6823.3F * n) + 5520.33F;

  /* Return the results in degrees Kelvin */
  return (uint16_t)cct;
}

/*!
 *  @brief  Converts the raw R/G/B values to color temperature in degrees
 *          Kelvin using the algorithm described in DN40 from Taos (now AMS).
 *  @param  r
 *          Red value
 *  @param  g
 *          Green value
 *  @param  b
 *          Blue value
 *  @param  c
 *          Clear channel value
 *  @return Color temperature in degrees Kelvin
 */
uint16_t tcs34725_calculateColorTemperature_dn40(uint16_t r,
                                                 uint16_t g,
                                                 uint16_t b,
                                                 uint16_t c)
{
  uint16_t r2, b2; /* RGB values minus IR component */
  uint16_t sat;    /* Digital saturation level */
  uint16_t ir;     /* Inferred IR content */

  if (c == 0)
  {
    return 0;
  }

  /* Analog/Digital saturation:
   *
   * (a) As light becomes brighter, the clear channel will tend to
   *     saturate first since R+G+B is approximately equal to C.
   * (b) The TCS34725 accumulates 1024 counts per 2.4ms of integration
   *     time, up to a maximum values of 65535. This means analog
   *     saturation can occur up to an integration time of 153.6ms
   *     (64*2.4ms=153.6ms).
   * (c) If the integration time is > 153.6ms, digital saturation will
   *     occur before analog saturation. Digital saturation occurs when
   *     the count reaches 65535.
   */
  if ((256 - tcs34725._integrationTime) > 63)
  {
    /* Track digital saturation */
    sat = 65535;
  }
  else
  {
    /* Track analog saturation */
    sat = 1024 * (256 - tcs34725._integrationTime);
  }

  /* Ripple rejection:
   *
   * (a) An integration time of 50ms or multiples of 50ms are required to
   *     reject both 50Hz and 60Hz ripple.
   * (b) If an integration time faster than 50ms is required, you may need
   *     to average a number of samples over a 50ms period to reject ripple
   *     from fluorescent and incandescent light sources.
   *
   * Ripple saturation notes:
   *
   * (a) If there is ripple in the received signal, the value read from C
   *     will be less than the max, but still have some effects of being
   *     saturated. This means that you can be below the 'sat' value, but
   *     still be saturating. At integration times >150ms this can be
   *     ignored, but <= 150ms you should calculate the 75% saturation
   *     level to avoid this problem.
   */
  if ((256 - tcs34725._integrationTime) <= 63)
  {
    /* Adjust sat to 75% to avoid analog saturation if atime < 153.6ms */
    sat -= sat / 4;
  }

  /* Check for saturation and mark the sample as invalid if true */
  if (c >= sat)
  {
    return 0;
  }

  /* AMS RGB sensors have no IR channel, so the IR content must be */
  /* calculated indirectly. */
  ir = (r + g + b > c) ? (r + g + b - c) / 2 : 0;

  /* Remove the IR component from the raw RGB values */
  r2 = r - ir;
  b2 = b - ir;

  if (r2 == 0)
  {
    return 0;
  }

  /* A simple method of measuring color temp is to use the ratio of blue */
  /* to red light, taking IR cancellation into account. */
  uint16_t cct = (3810 * (uint32_t)b2) / /** Color temp coefficient. */
                     (uint32_t)r2 +
                 1391; /** Color temp offset. */

  return cct;
}

/*!
 *  @brief  Converts the raw R/G/B values to lux
 *  @param  r
 *          Red value
 *  @param  g
 *          Green value
 *  @param  b
 *          Blue value
 *  @return Lux value
 */
uint16_t tcs34725_calculateLux(uint16_t r, uint16_t g, uint16_t b)
{
  float illuminance;

  /* This only uses RGB ... how can we integrate clear or calculate lux */
  /* based exclusively on clear since this might be more reliable?      */
  illuminance = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);

  return (uint16_t)illuminance;
}

/*!
 * @brief Converts the raw RGBW values to Photosynthetically Active Radiation(PAR)
 * https://ledgardener.com/forum/viewtopic.php?t=5748
 *  @param  w
 *          White value
 *  @param  r
 *          Red value
 *  @param  g
 *          Green value
 *  @param  b
 *          Blue value
 *  @return PAR value
 */

uint16_t tcs34725_calculatePAR(uint16_t w, uint16_t r, uint16_t g, uint16_t b)
{
  float cf_lux = 2.0;
  float cf_par = 0.015;
  float par_value;
  float luminance = (-0.32466 * r) + (1.57837 * g) + (-0.73191 * b);
  float lux = luminance * cf_lux;
  par_value = lux*cf_par;

  return (uint16_t)par_value;
}

/*!
 *  @brief  Sets inerrupt for TCS34725
 *  @param  i
 *          Interrupt (True/False)
 */
void tcs34725_setInterrupt(bool i)
{
  uint8_t r;
  io_tcs34725._read(TCS34725_ENABLE, &r, 1);
  if (i)
  {
    r |= TCS34725_ENABLE_AIEN;
  }
  else
  {
    r &= ~TCS34725_ENABLE_AIEN;
  }
  io_tcs34725._write(TCS34725_ENABLE, r);
}

/*!
 *  @brief  Clears inerrupt for TCS34725
 */
void tcs34725_clearInterrupt()
{
  io_tcs34725._write(0x66, 0);
}

/*!
 *  @brief  Sets inerrupt limits
 *  @param  low
 *          Low limit
 *  @param  high
 *          High limit
 */
void tcs34725_setIntLimits(uint16_t low, uint16_t high)
{
  io_tcs34725._write(0x04, low & 0xFF);
  io_tcs34725._write(0x05, low >> 8);
  io_tcs34725._write(0x06, high & 0xFF);
  io_tcs34725._write(0x07, high >> 8);
}


/*!
 *  @brief  Initializes a struct for flicker detection
 *  @param  flicker_struct 
 *          a structure for holding of information, you must initialize a structure
 *          for each color channel you want to monitor for flicker
 *  @param  intensity_treschold
 *          How much intensity must change in order to detect it as flicker
 *  @param  count_treschold
 *          How many of that changes need to happen to set the flicker flag to true
 *          
 *          Flicker detection is not supported by hardware so it is rather slow 
 *          and can't detect high frequency light flickering as it only checks for changes
 *          in intensity of light in selected channel on each reading.
 */
void tcs34725_flickerInit(tcs34725Flicker_t * flicker_struct, uint16_t intensity_treschold, uint8_t count_treschold)
{
  flicker_struct->_count_treschold=count_treschold;
  flicker_struct->_intensity_treschold=intensity_treschold;
}

/*!
 *  @brief  Checks for flicker on selected color channel
 *  @param  flicker_struct 
 *          a structure for holding of information, you must initialize a structure
 *          for each color channel you want to monitor for flicker
 *  @param  channel_intensity
 *          Provide the raw data according to channel you are currently checking
 */
bool tcs34725_checkFlicker(tcs34725Flicker_t * flicker_struct, uint16_t channel_intensity)
{
  bool result;
  if((channel_intensity>(flicker_struct->_lastValue+flicker_struct->_intensity_treschold)) ||
      (channel_intensity<(flicker_struct->_lastValue-flicker_struct->_intensity_treschold)))
  {
    flicker_struct->_flicks_count++;
  }
  else if(flicker_struct->_flicks_count!=0){
   flicker_struct->_flicks_count--;
  }
  else flicker_struct->_did_flick = false;
  flicker_struct->_lastValue = channel_intensity;
  if(flicker_struct->_flicks_count>=flicker_struct->_count_treschold)
  {
    flicker_struct->_flicks_count=flicker_struct->_count_treschold;
    result = true;
    flicker_struct->_did_flick = true;
  }
  else if (flicker_struct->_did_flick)
    result = true;
  return result;
}
