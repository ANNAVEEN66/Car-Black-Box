/*
 * File:   external_eeprom.c
 * Author: user
 *
 * Created on October 29, 2024, 3:58 PM
 */


#include <xc.h>
#include "external_eeprom.h"
#include "i2c.h"

// Function to write a byte to the EEPROM at a given address
void write_external_eeprom(unsigned char address, unsigned char data)
{
    i2c_start();
    i2c_write(SLAVE_WRITE_E);    // Send EEPROM write address
    i2c_write(address);           // Send the EEPROM memory address to write to
    i2c_write(data);              // Send the data byte
    i2c_stop();                   // Stop I2C communication

    // Delay for EEPROM write cycle time
    for (unsigned int wait = 3000; wait--;);
}

// Function to read a byte from the EEPROM at a given address
unsigned char read_external_eeprom(unsigned char address)
{
    unsigned char data;

    i2c_start();
    i2c_write(SLAVE_WRITE_E);    // Send EEPROM write address
    i2c_write(address);           // Send the EEPROM memory address to read from
    i2c_rep_start();              // Repeat start for reading
    i2c_write(SLAVE_READ_E);      // Send EEPROM read address
    data = i2c_read();            // Read data byte from EEPROM
    i2c_stop();                   // Stop I2C communication

    return data;
}
