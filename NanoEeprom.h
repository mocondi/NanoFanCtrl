#pragma once

#define NANO_EEPROM_SIZE    1024
#define SERIAL_OFFSET       NANO_EEPROM_SIZE - 4// 4 bytes at the end

namespace NANO_EEPROM
{
    bool defaulted();
    void readTable(void* ptr);
    void updateTable(void* ptr);

}