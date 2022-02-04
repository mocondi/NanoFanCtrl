/*
* NanoEeprom.cpp
* Handles reading and writing to local EEPROM memory
*
*/
#include <Arduino.h>
#include <EEPROM.h>
#include "NanoEeprom.h"

bool NANO_EEPROM::defaulted()
{
    int serial = EEPROM.get(SERIAL_OFFSET);
}

void NANO_EEPROM::readTable(void* ptr)
{

}

void NANO_EEPROM::updateTable(void* ptr)
{

}
