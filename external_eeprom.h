/* 
 * File:   external_eeprom.h
 * Author: user
 *
 * Created on October 29, 2024, 3:58 PM
 */

#ifndef EXTERNAL_EEPROM_H
#define EXTERNAL_EEPROM_H

#define SLAVE_READ_E  0xA1   // EEPROM read address
#define SLAVE_WRITE_E 0xA0   // EEPROM write address

// Function prototypes
void write_external_eeprom(unsigned char address, unsigned char data);
unsigned char read_external_eeprom(unsigned char address);

#endif // EXTERNAL_EEPROM_H

