#ifndef __INEEPROM_INTERFACE_H__
#define __INEEPROM_INTERFACE_H__

#define EEPE_BIT          (1)
#define Enable_read      (0x01)
#define EECR_EEMWE        (2) 
#define EECR_EEWE         (1)

void ineeprom_write(u16_t adderess, u8_t data);



void ineeprom_read(u16_t adderess, u16_t *read);




#endif /*__INEEPROM_INTERFACE_H__*/