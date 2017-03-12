#ifndef ASSERV_MOTORSCONTROLLER_MD25_MD25_H_
#define ASSERV_MOTORSCONTROLLER_MD25_MD25_H_

/********************************************************************
 * @filename    MD25.h
 * @author      Thomas B. Joergensen (thomas.bal.jorgensen@gmail.com)
 * @date        06 FEB 2012
 * @version     1.0
 * @target      mbed (NXP LPC1768 - ARM Cortex M3 - 32-bit)
 *
 * @desciption  A library for interacting with the MD25 DC motor
 *              controller. Find more at:
 *              http://www.robot-electronics.co.uk/htm/md25tech.htm
 *              http://www.robot-electronics.co.uk/htm/md25i2c.htm
 *******************************************************************/

/* Includes */
#include "mbed.h"

/* REGISTERS */
#define REG_SPEED1                  0x00    // R/W  | Motor1 speed (mode 0,1) or speed (mode 2,3)
#define REG_SPEED2                  0x01    // R/W  | Motor2 speed (mode 0,1) or turn (mode 2,3)
#define REG_ENC1A                   0x02    // R    | Encoder 1 position, 1st byte (highest), capture count when read
#define REG_ENC1B                   0x03    // R    | Encoder 1 position, 2nd byte
#define REG_ENC1C                   0x04    // R    | Encoder 1 position, 3rd byte
#define REG_ENC1D                   0x05    // R    | Encoder 1 position, 4th (lowest byte)
#define REG_ENC2A                   0x06    // R    | Encoder 2 position, 1st  byte (highest), capture count when read
#define REG_ENC2B                   0x07    // R    | Encoder 2 position, 2nd byte
#define REG_ENC2C                   0x08    // R    | Encoder 2 position, 3rd byte
#define REG_ENC2D                   0x09    // R    | Encoder 2 position, 4th byte (lowest byte)
#define REG_BATTERY_VOLTS           0x0A    // R    | The supply battery voltage
#define REG_MOTOR1_CURRENT          0x0B    // R    | The current through motor 1
#define REG_MOTOR2_CURRENT          0x0C    // R    | The current through motor 2
#define REG_SOFTWARE_REVISION       0x0D    // R    | Software Revision Number
#define REG_ACCELERATION_RATE       0x0E    // R/W  | Optional Acceleration register
#define REG_MODE                    0x0F    // R/W  | Mode of operation (see below)
#define REG_COMMAND                 0x10    // R/W  | Used for reset of encoder counts and module address changes

/* MODES */
#define MODE_0                      0x00    // The meaning of the speed registers is literal speeds in the range of 0 (Full Reverse), 128 (Stop), 255 (Full Forward) (Default Setting).
#define MODE_1                      0x01    // The meaning of the speed registers is literal speeds in the range of -128 (Full Reverse), 0 (Stop), 127 (Full Forward).
#define MODE_2                      0x02    // Speed1 control both motors speed, and speed2 becomes the turn value. Data is in the range of 0 (Full Reverse), 128 (Stop), 255 (Full  Forward).
#define MODE_3                      0x03    // Speed1 control both motors speed, and speed2 becomes the turn value. Data is in the range of -128 (Full Reverse), 0 (Stop), 127 (Full Forward).

/* COMMANDS */
#define CMD_ENCODER_RESET           0x20    // Resets the encoder registers to zero
#define CMD_AUTO_SPEED_DISABLE      0x30    // Disables automatic speed regulation
#define CMD_AUTO_SPEED_ENABLE       0x31    // Enables automatic speed regulation (default)
#define CMD_TIMEOUT_DISABLE         0x32    // Disables 2 second timeout of motors (Version 2 onwards only)
#define CMD_TIMEOUT_ENABLE          0x33    // Enables 2 second timeout of motors when no I2C comms (default) (Version 2 onwards only)
#define CMD_CHANGE_I2C_ADDR_1       0xA0    // 1st in sequence to change I2C address
#define CMD_CHANGE_I2C_ADDR_2       0xAA    // 2nd in sequence to change I2C address
#define CMD_CHANGE_I2C_ADDR_3       0xA5    // 3rd in sequence to change I2C address

/* I2C ADDRESS */
#define I2C_START_ADDR              0xB0    // The start address of valid I2C addresses. LSB indicates R/W, so add 2 to get next valid. Last valid is 0xBE.
#define I2C_WRITE_BIT               0x00    // Add this to I2C address to perform a write.
#define I2C_READ_BIT                0x01    // Add this to I2C address to perform a read.

class MD25 {
private:
    /* Communication */
    I2C *i2c_interface;

    /* Variables */
    int mode;
    char i2c_address;

    /* Generic methods */
    int write(char reg_addr, char *data, int bytes);
    int read(char reg_addr, char *data, int bytes);
    char doDiscover();
public:
    /* Constructor and Destructor */
    MD25(I2C *i2c_interface, char i2c_address);
    ~MD25(void);

    /* Control methods */
    int mode_set(int mode);                 // Set the mode of operation (0 - 3) (default: 0).
    int mode_get();                         // Get the current mode of operation.
    int encoder_reset();                    // Resets the encoder registers.
    int auto_speed_set(bool enabled);       // Enable/disable automatic speed regulation (default: enabled).
    int timeout_set(bool enabled);          // Enable/disable 2 sec timeout of motors when no I2C comms (default: enabled).
    int i2c_addr_set(char address);         // Set a new I2C device address (Default: 0xB0).

    /* Data methods */
    int speed1_set(int speed);              // Set the speed of motor 1. (Only mode 0 or 1).
    int speed1_get();                       // Get the set speed of motor 1. (Only mode 0 or 1).
    int speed2_set(int speed);              // Set the speed of motor 2. (Only mode 0 or 1).
    int speed2_get();                       // Get the set speed of motor 2. (Only mode 0 or 1).

    int speed_set(int speed);               // Set the speed. (Only mode 2 or 3).
    int speed_get();                        // Get the set speed. (Only mode 2 or 3).
    int turn_set(int turn);                 // Set the turn speed. (Only mode 2 or 3).
    int turn_get();                         // Get the set turn speed. (Only mode 2 or 3).

    int acceleration_set(int acceleration); // Set a desired acceleration rate.
    int acceleration_get();                 // Get the set desired acceleration rate.

    int encoder1_get();                     // Encoder 1 position.
    int encoder2_get();                     // Encoder 2 position.

    int bat_voltage_get();                // Get battery voltage x 10.
    int motor1_current_get();             // Get current for motor 1.
    int motor2_current_get();             // Get current for motor 2.

    int software_rev_num_get();             // Returns the software revision in the PIC16F873 controller.
};



#endif /* ASSERV_MOTORSCONTROLLER_MD25_MD25_H_ */
