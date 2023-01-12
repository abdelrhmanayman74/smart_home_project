/**
 * @file    HKPD_cfg.h
 * @author  Ahmed Ashraf (ahmedashrafelnaqeeb@gmail.com)
 * @brief   This file contains configuration information related to Keypad module
 * @version 1.0
 * @date    2022-09-24
 * 
 * 
 */

/*Header file guard*/
#ifndef __HKPD_CFG_H__
#define __HKPD_CFG_H__

/***************************************************************************************************/
/*                                         Configuration macros                                    */
/***************************************************************************************************/

/*Number of rows and columns configurations*/
#define KPD_NUM_ROWS        (4)
#define KPD_NUM_COLS        (4)

#define num0 (0)
#define num1 (1)
#define num2 (2)
#define num3 (3)
#define num4 (4)
#define num5 (5)
#define num6 (6)
#define num7 (7)
#define num8 (8)
#define num9 (9)

/*Keypad mapping configurations*/
#define KEYPAD_MAPPING      {                          \
                                {'1', '2', '3', 'A'},  \
                                {'4', '5', '6', 'B'},  \
                                {'7', '8', '9', 'C'},  \
                                {'R', '0', 'E', 'D'}   \
                            }

/*Keypad pins configurations*/
#define KPD_PORT            (PORTC)
#define KPD_PORT2           (PORTD)
#define KPD_PORT3           (PORTB)
#define KPD_PORT4           (PORTA)
#define KPD_ROW0            (PIN0)
#define KPD_ROW1            (PIN4)
#define KPD_ROW2            (PIN4)
#define KPD_ROW3            (PIN3)
#define KPD_COL0            (PIN4)
#define KPD_COL1            (PIN5)
#define KPD_COL2            (PIN6)
#define KPD_COL3            (PIN7)

#define KPD_DEFAULT_VALUE   (0)

#endif /*__HKPD_CFG_H__*/