#ifndef __INEEPROM_PRIVATE_H__
#define __INEEPROM_PRIVATE_H__


#define INEEPROM_EEAR                   (*(volatile u16_t*)(0x3E))
#define INEEPROM_EEDR                   (*(volatile u8_t*)(0x3D))
#define INEEPROM_EECR                   (*(volatile u8_t*)(0x3C))



#endif /*__INEEPROM_PRIVATE_H__*/