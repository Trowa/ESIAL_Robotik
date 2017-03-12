/*
 * Md25ctrl.h
 *
 *  Created on: 12 mars 2017
 *      Author: pmx
 */

#ifndef ASSERV_MOTORSCONTROLLER_MD25_MD25CTRL_H_
#define ASSERV_MOTORSCONTROLLER_MD25_MD25CTRL_H_

#include <I2C.h>
#include <PinNames.h>

#include "../MotorsController.h"
#include "MD25.h"

#define ADDR_MD25 0xB0

DigitalOut ErrorLed(LED1);


class Md25ctrl : public MotorsController
{
public:

    // Constructeur
	Md25ctrl(PinName I2CsdaPin , PinName I2CsclPin);
    // Destructeur
    ~Md25ctrl();

    // Paramétrage des vitesses
    virtual void vitesseG(int );
    virtual void vitesseD(int );
    virtual int getVitesseG() { return vitMoteurG_; }
    virtual int getVitesseD() { return vitMoteurD_; }

private:
    int vitMoteurG_, vitMoteurD_;
    // Communication I2C entre la Mbed et la MD22
    I2C i2cLink_;

    MD25 md_;

};

#endif /* ASSERV_MOTORSCONTROLLER_MD25_MD25CTRL_H_ */
