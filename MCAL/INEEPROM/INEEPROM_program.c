#include "LSTD_TYPES.h"
#include "LBIT_MATH.h"
#include "INEEPROM_private.h"
#include "INEEPROM_interface.h"



void ineeprom_write(u16_t adderess, u8_t data)
{
    while(GET_BIT(INEEPROM_EECR, EEPE_BIT) == 1);
    INEEPROM_EEAR = adderess;
    INEEPROM_EEDR = data;
    SET_BIT(INEEPROM_EECR, EECR_EEMWE);
    SET_BIT(INEEPROM_EECR, EECR_EEWE);
}

void ineeprom_read(u16_t adderess, u16_t* read)
{
    while(GET_BIT(INEEPROM_EECR, EEPE_BIT) == 1);
    INEEPROM_EEAR = adderess;
    INEEPROM_EECR |= Enable_read;
    *read = INEEPROM_EEDR;
   
}

