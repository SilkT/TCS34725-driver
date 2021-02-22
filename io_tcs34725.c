#include "io_tcs34725.h"
#include "drv_tcs34725.h"

io_tcs34725_t io_tcs34725 =  
{
    ._write = tcs34725_io_write, ._read = tcs32725_io_read
};

void tcs34725_io_write(uint8_t reg, uint8_t value)
{
    uint8_t reg_to_write = TCS34725_COMMAND_BIT | reg;
    uint8_t val_to_write = value & 0xFF;
    //implement writing function for your platform here
}
void tcs32725_io_read(uint8_t reg, uint8_t * rvalue, uint8_t len)
{
    uint8_t reg_to_write;
    reg_to_write = TCS34725_COMMAND_BIT | reg;
    //implement receiving function for your platform here     
}

void tcs32725_io_delay(uint32_t milliseconds)
{
    // implement delay function for your platform here
}