/* mbed Microcontroller Library - I2C
 * Copyright (c) 2007-2009 ARM Limited. All rights reserved.
 * sford
 */ 
 
#ifndef MBED_I2C_H
#define MBED_I2C_H

#include "platform.h"
#include "PinNames.h"
#include "PeripheralNames.h"
#include "Base.h"

namespace mbed {

/* Class: I2C
 *  An I2C Master, used for communicating with I2C slave devices
 *
 * Example:
 * > // Read from I2C slave at address 0x1234
 * >
 * > #include "mbed.h"
 * >
 * > I2C i2c(p28, p27);
 * >
 * > int main() {
 * >     int address = 0x62;
 * >     char data[2];
 * >     i2c.read(address, data, 2);
 * > }
 */
class I2C : public Base {

public:

    /* Constructor: I2C
     *  Create an I2C Master interface, connected to the specified pins
     *
     * Variables:
     *  sda - I2C data line pin
     *  scl - I2C clock line pin
     */
    I2C(PinName sda, PinName scl, const char *name = NULL);

    /* Function: frequency
     *  Set the frequency of the I2C interface
     *
     * Variables:
     *  hz - The bus frequency in hertz
     */
    void frequency(int hz);

    /* Function: read
     *  Read from an I2C slave
     *
     * Performs a complete read transaction. The bottom bit of
     * the address is forced to 1 to indicate a read.
     *
     * Variables:
     *  address - 8-bit I2C slave address [ addr | 1 ]
     *  data - Pointer to the byte-array to read data in to 
     *  length - Number of bytes to read
     *  returns - 0 on success (ack), or non-0 on failure (nack)
     */ 
    int read(int address, char *data, int length); 

    /* Function: write
     *  Write to an I2C slave
     *
     * Performs a complete write transaction. The bottom bit of
     * the address is forced to 0 to indicate a write.
     *
     * Variables:
     *  address - 8-bit I2C slave address [ addr | 0 ]
     *  data - Pointer to the byte-array data to send 
     *  length - Number of bytes to send
     *  returns - 0 on success (ack), or non-0 on failure (nack)
     */ 
    int write(int address, const char *data, int length);
    
protected:

    I2CName _i2c;
    
    void aquire();
    static I2C *_owner;
    int _hz;

};

} // namespace mbed

#endif
