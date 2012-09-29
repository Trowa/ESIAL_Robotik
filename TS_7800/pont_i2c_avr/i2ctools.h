#ifndef I2CTOOLS_H
#define I2CTOOLS_H

#include <avr/io.h>

void i2c_init(void);
int i2c_start(void);
int i2c_write_addr(uint8_t address);
int i2c_write_data(uint8_t data);
int i2c_stop(void);

#endif //I2CTOOLS_H
