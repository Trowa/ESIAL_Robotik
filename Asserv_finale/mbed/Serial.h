/* mbed Microcontroller Library - Serial
 * Copyright (c) 2007-2009 ARM Limited. All rights reserved.
 * sford
 */ 
 
#ifndef MBED_SERIAL_H
#define MBED_SERIAL_H

#include "platform.h"
#include "PinNames.h"
#include "PeripheralNames.h"
#include "Stream.h"
#include "FunctionPointer.h"

namespace mbed {

/* Class: Serial
 *  A serial port (UART) for communication with other serial devices
 *
 * Example:
 * > // Print "Hello World" to the PC
 * >
 * > #include "mbed.h"
 * >
 * > Serial pc(USBTX, USBRX);
 * >
 * > int main() {
 * >     pc.printf("Hello World\n");
 * > }
 */
class Serial : public Stream {

public:

    /* Constructor: Serial
     *  Create a Serial port, connected to the specified transmit and receive pins
     *
     * Variables:
     *  tx - Transmit pin 
     *  rx - Receive pin
     *
     *  Note: Either tx or rx may be specified as NOT_CONNECTED if unused
     */
    Serial(PinName tx, PinName rx, const char *name = NULL);

    /* Function: baud
     *  Set the baud rate of the serial port
     *  
     * Variables:
     *  baudrate - The baudrate of the serial port (default = 9600).
     */
    void baud(int baudrate);

    enum Parity {
        None = 0
        , Odd
        , Even
        , Forced1    
        , Forced0
    };

    /* Function: format
     *  Set the transmission format used by the Serial port
     *
     * Variables:
     *  bits - The number of bits in a word (5-8; default = 8)
     *  parity - The parity used (Serial::None, Serial::Odd, Serial::Even, Serial::Forced1, Serial::Forced0; default = Serial::None)
     *  stop - The number of stop bits (1 or 2; default = 1)
     */
    void format(int bits = 8, Parity parity = Serial::None, int stop_bits = 1); 

#if 0 // Inhereted from Stream, for documentation only

    /* Function: putc
     *  Write a character
     *
     * Variables:
     *  c - The character to write to the serial port
     */
    int putc(int c);

    /* Function: getc
     *  Read a character
     *
     * Variables:
     *  returns - The character read from the serial port
     */
    int getc();

    /* Function: printf
     *  Write a formated string
     *
     * Variables:
     *  format - A printf-style format string, followed by the 
     *      variables to use in formating the string.
     */
    int printf(const char* format, ...);

    /* Function: scanf
     *  Read a formated string 
     *
     * Variables:
     *  format - A scanf-style format string,
     *      followed by the pointers to variables to store the results. 
     */
    int scanf(const char* format, ...);
 
#endif
 
    /* Function: readable
     *  Determine if there is a character available to read
     *
     * Variables:
     *  returns - 1 if there is a character available to read, else 0
     */
    int readable();

    /* Function: writeable
     *  Determine if there is space available to write a character
     * 
     * Variables:
     *  returns - 1 if there is space to write a character, else 0
     */
    int writeable();

    /* Function: attach
     *  Attach a function to call whenever a serial interrupt is generated
     *
     * Variables:
     *  fptr - A pointer to a void function, or 0 to set as none
     */
    void attach(void (*fptr)(void));

    /* Function: attach
     *  Attach a member function to call whenever a serial interrupt is generated
     *     
     * Variables:
     *  tptr - pointer to the object to call the member function on
     *  mptr - pointer to the member function to be called
     */
    template<typename T>
    void attach(T* tptr, void (T::*mptr)(void)) {
        _irq.attach(tptr, mptr);
        setup_interrupt();
    }

#ifdef MBED_RPC
    virtual const struct rpc_method *get_rpc_methods();
    static struct rpc_class *get_rpc_class();
#endif

protected:

    void setup_interrupt();
    void remove_interrupt();

    virtual int _getc();
    virtual int _putc(int c);

    UARTName _uart;
    FunctionPointer _irq;

};

} // namespace mbed

#endif

