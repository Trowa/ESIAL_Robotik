#ifndef I2CTOOLS_H
#define I2CTOOLS_H

#include <avr/io.h>

int i2c_start(void);
int i2c_write(uint8_t data);
int i2c_stop(void);

#endif //I2CTOOLS_H
