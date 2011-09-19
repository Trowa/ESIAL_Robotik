/* mbed Microcontroller Library - PwmOut
 * Copyright (c) 2007-2009 ARM Limited. All rights reserved.
 * sford
 */ 
 
#ifndef MBED_PWMOUT_H
#define MBED_PWMOUT_H

#include "platform.h"
#include "PinNames.h"
#include "PeripheralNames.h"
#include "Base.h"

namespace mbed {

/* Class: PwmOut
 *  A pulse-width modulation digital output
 *
 * Example
 * > // Fade a led on.
 * > #include "mbed.h"
 * >
 * > PwmOut led(LED1);
 * > 
 * > int main() {
 * >     while(1) {
 * >         led = led + 0.01;
 * >         wait(0.2);
 * >         if(led == 1.0) {
 * >             led = 0;
 * >         }
 * >     }
 * > }
 */
class PwmOut : public Base {

public:

    /* Constructor: PwmOut
     *  Create a PwmOut connected to the specified pin
     *
     * Variables:
     *  pin - PwmOut pin to connect to
     */
    PwmOut(PinName pin, const char *name = NULL);

    /* Function: write
     *  Set the ouput duty-cycle, specified as a percentage (float)
     *
     * Variables:
     *  value - A floating-point value representing the output duty-cycle, 
     *    specified as a percentage. The value should lie between
     *    0.0f (representing on 0%) and 1.0f (representing on 100%).
     *    Values outside this range will be saturated to 0.0f or 1.0f.	 
     */
    void write(float value);

    /* Function: read
     *  Return the current output duty-cycle setting, measured as a percentage (float)
     *
     * Variables:
     *  returns - A floating-point value representing the current duty-cycle being output on the pin, 
     *    measured as a percentage. The returned value will lie between
     *    0.0f (representing on 0%) and 1.0f (representing on 100%).
     *
     * Note:
     *  This value may not match exactly the value set by a previous <write>.
     */
    float read();
    
    /* Function: period
     *  Set the PWM period, specified in seconds (float)
     */
    void period(float seconds);

    /* Function: period_ms
     *  Set the PWM period, specified in milli-seconds (int)
     */
    void period_ms(int ms);

    /* Function: period_us
     *  Set the PWM period, specified in micro-seconds (int)
     */
    void period_us(int us);

    /* Function: pulsewidth
     *  Set the PWM pulsewidth, specified in seconds (float)
     */
    void pulsewidth(float seconds);

    /* Function: pulsewidth_ms
     *  Set the PWM pulsewidth, specified in milli-seconds (int)
     */
    void pulsewidth_ms(int ms);

    /* Function: pulsewidth_us
     *  Set the PWM pulsewidth, specified in micro-seconds (int)
     */
    void pulsewidth_us(int us);

#ifdef MBED_OPERATORS
    /* Function: operator=
     *  A operator shorthand for <write()>
     */
    PwmOut& operator= (float value);
    PwmOut& operator= (PwmOut& rhs);

    /* Function: operator float()
     *  An operator shorthand for <read()>
     */
    operator float();
#endif

#ifdef MBED_RPC
    virtual const struct rpc_method *get_rpc_methods();
    static struct rpc_class *get_rpc_class();
#endif

protected:

    PWMName _pwm;

};

} // namespace mbed

#endif
