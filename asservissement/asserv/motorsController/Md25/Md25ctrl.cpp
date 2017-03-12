/*
 * Md25ctrl.cpp
 *
 *  Created on: 12 mars 2017
 *      Author: pmx
 */

#include "Md25ctrl.h"

// Constructeur d'une instance de MD22
Md25ctrl::Md25ctrl(PinName I2CsdaPin, PinName I2CsclPin) :
		i2cLink_(I2CsdaPin, I2CsclPin), md_(&i2cLink_, ADDR_MD25)
{
	vitMoteurG_ = 0;
	vitMoteurD_ = 0;

	// Communication I2C entre la Mbed et la MD25
	i2cLink_.frequency(400000); //Hz
	//i2cLink_.start();
	int soft = md_.software_rev_num_get();

	int bat = md_.bat_voltage_get();

	md_.encoder_reset();
	int enc2 = md_.encoder2_get();
	int enc1 = md_.encoder1_get();
	int cur1 = md_.motor1_current_get();
	int cur2 = md_.motor2_current_get();

	printf("MD25 -- soft=%d \tbat=%d \tenc1=%d \tenc2=%d \tcur1=%d \tcur2=%d\r\n", soft, bat, enc1,
			enc2, cur1, cur2);

	int r = 0;
	r = md_.mode_set(MODE_1);
	if (r == -1)
	{
		printf("ERROR MD25 Mode not set %d\r\n", r);
		ErrorLed = 1;
	}
	r = md_.acceleration_set(10);
	if (r == -1)
	{
		printf("ERROR MD25 acceleration not set %d\r\n", r);
		ErrorLed = 1;
	}
	r = md_.auto_speed_set(true);
	if (r == -1)
	{
		printf("ERROR MD25 auto_speed not set %d\r\n", r);
		ErrorLed = 1;
	}
	r = md_.speed1_set(0);
	if (r == -1)
	{
		printf("ERROR MD25 speed1_set not set %d\r\n", r);
		ErrorLed = 1;
	}
	r = md_.speed2_set(0);
	if (r == -1)
	{
		printf("ERROR MD25 speed2_set not set %d\r\n", r);
		ErrorLed = 1;
	}

}

Md25ctrl::~Md25ctrl()
{
	// On arrête les moteurs à la destruction
	vitesseG(0);
	vitesseD(0);
	i2cLink_.stop();
}

void Md25ctrl::vitesseG(int vitMoteurG)
{
	if (Config::reglageCodeurs)
		return;

	if (Config::inverseMoteurG)
	{
		vitMoteurG = -vitMoteurG;
	}

	if (vitMoteurG > Config::V_MAX_POS_MOTOR)
	{
		vitMoteurG = Config::V_MAX_POS_MOTOR;
	}
	else if (vitMoteurG > 0 && vitMoteurG < Config::V_MIN_POS_MOTOR)
	{
		vitMoteurG = 0;
	}

	if (vitMoteurG < Config::V_MAX_NEG_MOTOR)
	{
		vitMoteurG = Config::V_MAX_NEG_MOTOR;
	}
	else if (vitMoteurG < 0 && vitMoteurG > Config::V_MIN_NEG_MOTOR)
	{
		vitMoteurG = 0;
	}

	if (!Config::swapMoteurs)
	{
		md_.speed1_set(vitMoteurG);
	}
	else
	{
		md_.speed2_set(vitMoteurG);
	}

	this->vitMoteurG_ = vitMoteurG;
}

void Md25ctrl::vitesseD(int vitMoteurD)
{
	if (Config::reglageCodeurs)
		return;

	if (Config::inverseMoteurD)
	{
		vitMoteurD = -vitMoteurD;
	}

	if (vitMoteurD > Config::V_MAX_POS_MOTOR)
	{
		vitMoteurD = Config::V_MAX_POS_MOTOR;
	}
	else if (vitMoteurD > 0 && vitMoteurD < Config::V_MIN_POS_MOTOR)
	{
		vitMoteurD = 0;
	}

	if (vitMoteurD < Config::V_MAX_NEG_MOTOR)
	{
		vitMoteurD = Config::V_MAX_NEG_MOTOR;
	}
	else if (vitMoteurD < 0 && vitMoteurD > Config::V_MIN_NEG_MOTOR)
	{
		vitMoteurD = 0;
	}

	if (!Config::swapMoteurs)
	{
		md_.speed2_set(vitMoteurD);
	}
	else
	{
		md_.speed1_set(vitMoteurD);
	}

	this->vitMoteurD_ = vitMoteurD;
}
