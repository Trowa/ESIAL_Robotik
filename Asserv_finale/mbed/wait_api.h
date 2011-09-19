/* mbed Microcontroller Library - wait_api
 * Copyright (c) 2009 ARM Limited. All rights reserved.
 * sford
 */ 
 
// GENERIC

#ifndef MBED_WAIT_API_H
#define MBED_WAIT_API_H

#ifdef __cplusplus
extern "C" {
#endif

void wait(float s);
void wait_ms(int ms);
void wait_us(int us);

#ifdef __cplusplus
}
#endif

#endif
