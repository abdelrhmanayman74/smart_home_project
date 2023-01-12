/**
 * @file    HKPD_program.c
 * @author  Ahmed Ashraf (ahmedashrafelnaqeeb@gmail.com)
 * @brief   This file contains logical implementation related to Keypad module
 * @version 1.0
 * @date    2022-09-24
 * 
 * 
 */

/***************************************************************************************************/
/*                                             Includes                                            */
/***************************************************************************************************/

#include "LSTD_TYPES.h"
#include "LBIT_MATH.h"
#include "MDIO_interface.h"
#include "HKPD_cfg.h"
#include "HKPD_interface.h"
#include "util/delay.h"

/***************************************************************************************************/
/*                                          Important macros                                       */
/***************************************************************************************************/

#define DEBOUNCE_TIME_MS      (30)

/***************************************************************************************************/
/*                                          Important variables                                    */
/***************************************************************************************************/

/*Static global 2-D Array used to store the mapping of the keypad*/
static u8_t garr_keypadMapping[KPD_NUM_ROWS][KPD_NUM_COLS] = KEYPAD_MAPPING;

/***************************************************************************************************/
/*                                       Functions' definitions                                    */
/***************************************************************************************************/

void hkpd_init(void)
{
    /*Setting the rows as input pullup*/
    mdio_setPinStatus(KPD_PORT4, KPD_ROW0, INPUT_PULLUP);
    mdio_setPinStatus(KPD_PORT3, KPD_ROW1, INPUT_PULLUP);
    mdio_setPinStatus(KPD_PORT2, KPD_ROW2, INPUT_PULLUP);
    mdio_setPinStatus(KPD_PORT, KPD_ROW3, INPUT_PULLUP);

    /*Setting the columns as output*/
    mdio_setPinStatus(KPD_PORT, (KPD_COL0 | KPD_COL1 | KPD_COL2), OUTPUT);
    mdio_setPinStatus(KPD_PORT3, KPD_COL3, OUTPUT);

    /*Setting the columns as output high*/
    mdio_setPinValue(KPD_PORT, (KPD_COL0 | KPD_COL1 | KPD_COL2), HIGH);
    mdio_setPinValue(KPD_PORT3, KPD_COL3, HIGH);

    /*Return from this function*/
    return;
}

void hkpd_getPressedButton(u8_t* pu8_button)
{
    *pu8_button=KPD_DEFAULT_VALUE;
    u8_t au8_returnValue;

    mdio_setPinValue(KPD_PORT,KPD_COL0,LOW);
    mdio_getPinValue(KPD_PORT4,KPD_ROW0,&au8_returnValue);
    if (au8_returnValue==LOW)
    {
        _delay_ms(30);
        *pu8_button=garr_keypadMapping[0][0];
        mdio_setPinValue(KPD_PORT,KPD_COL0,HIGH);
        return;

    }
    mdio_getPinValue(KPD_PORT3,KPD_ROW1,&au8_returnValue);
    if (au8_returnValue==LOW)
    {
        _delay_ms(30);
        *pu8_button=garr_keypadMapping[1][0];
        mdio_setPinValue(KPD_PORT,KPD_COL0,HIGH);
        return ;
    }
     mdio_getPinValue(KPD_PORT2,KPD_ROW2,&au8_returnValue);
    if (au8_returnValue==LOW)
    {
        _delay_ms(30);
        *pu8_button=garr_keypadMapping[2][0];
        mdio_setPinValue(KPD_PORT,KPD_COL0,HIGH);
        return;
    }
    mdio_getPinValue(KPD_PORT,KPD_ROW3,&au8_returnValue);
    if (au8_returnValue==LOW)
    {
        _delay_ms(30);
        *pu8_button=garr_keypadMapping[3][0];
        mdio_setPinValue(KPD_PORT,KPD_COL0,HIGH);
        return ;
    }

    mdio_setPinValue(KPD_PORT,KPD_COL0,HIGH);
    mdio_setPinValue(KPD_PORT,KPD_COL1,LOW);
    mdio_getPinValue(KPD_PORT4,KPD_ROW0,&au8_returnValue);
    if (au8_returnValue==LOW)
    {
        _delay_ms(30);
        *pu8_button=garr_keypadMapping[0][1];
        mdio_setPinValue(KPD_PORT,KPD_COL1,HIGH);
        return ;

    }
    mdio_getPinValue(KPD_PORT3,KPD_ROW1,&au8_returnValue);
    if (au8_returnValue==LOW)
    {
        _delay_ms(30);
        *pu8_button=garr_keypadMapping[1][1];
        mdio_setPinValue(KPD_PORT,KPD_COL1,HIGH);
        return ;
    }
     mdio_getPinValue(KPD_PORT2,KPD_ROW2,&au8_returnValue);
    if (au8_returnValue==LOW)
    {
        _delay_ms(30);
        *pu8_button=garr_keypadMapping[2][1];
        mdio_setPinValue(KPD_PORT,KPD_COL1,HIGH);
        return;
    }
    mdio_getPinValue(KPD_PORT,KPD_ROW3,&au8_returnValue);
    if (au8_returnValue==LOW)
    {
        _delay_ms(30);
        *pu8_button=garr_keypadMapping[3][1];
        mdio_setPinValue(KPD_PORT,KPD_COL1,HIGH);
        return ;
    }

    mdio_setPinValue(KPD_PORT,KPD_COL1,HIGH);
    mdio_setPinValue(KPD_PORT,KPD_COL2,LOW);

    mdio_getPinValue(KPD_PORT4,KPD_ROW0,&au8_returnValue);
    if (au8_returnValue==LOW)
    {
        _delay_ms(30);
        *pu8_button=garr_keypadMapping[0][2];
        mdio_setPinValue(KPD_PORT,KPD_COL2,HIGH);
        return ;

    }
    mdio_getPinValue(KPD_PORT3,KPD_ROW1,&au8_returnValue);
    if (au8_returnValue==LOW)
    {
        _delay_ms(30);
        *pu8_button=garr_keypadMapping[1][2];
        mdio_setPinValue(KPD_PORT,KPD_COL2,HIGH);
        return;
    }
     mdio_getPinValue(KPD_PORT2,KPD_ROW2,&au8_returnValue);
    if (au8_returnValue==LOW)
    {
        _delay_ms(30);
        *pu8_button=garr_keypadMapping[2][2];
        mdio_setPinValue(KPD_PORT,KPD_COL2,HIGH);
        return;
    }
    mdio_getPinValue(KPD_PORT,KPD_ROW3,&au8_returnValue);
    if (au8_returnValue==LOW)
    {
        _delay_ms(30);
        *pu8_button=garr_keypadMapping[3][2];
        mdio_setPinValue(KPD_PORT,KPD_COL2,HIGH);
        return;
    }

    mdio_setPinValue(KPD_PORT,KPD_COL2,HIGH);
    mdio_setPinValue(KPD_PORT3,KPD_COL3,LOW);

    mdio_getPinValue(KPD_PORT4,KPD_ROW0,&au8_returnValue);
    if (au8_returnValue==LOW)
    {
        _delay_ms(30);
        *pu8_button=garr_keypadMapping[0][3];
        mdio_setPinValue(KPD_PORT3,KPD_COL3,HIGH);
        return ;

    }
    mdio_getPinValue(KPD_PORT3,KPD_ROW1,&au8_returnValue);
    if (au8_returnValue==LOW)
    {
        _delay_ms(30);
        *pu8_button=garr_keypadMapping[1][3];
        mdio_setPinValue(KPD_PORT3,KPD_COL3,HIGH);
        return ;
    }
     mdio_getPinValue(KPD_PORT2,KPD_ROW2,&au8_returnValue);
    if (au8_returnValue==LOW)
    {
        _delay_ms(30);
        *pu8_button=garr_keypadMapping[2][3];
        mdio_setPinValue(KPD_PORT3,KPD_COL3,HIGH);
        return ;
    }
    mdio_getPinValue(KPD_PORT,KPD_ROW3,&au8_returnValue);
    if (au8_returnValue==LOW)
    {
        _delay_ms(30);
        *pu8_button=garr_keypadMapping[3][3];
        mdio_setPinValue(KPD_PORT3,KPD_COL3,HIGH);
        return ;
    }
    mdio_setPinValue(KPD_PORT3,KPD_COL3,HIGH);

    return;


}
void hkpd_get_int_of_Button(u8_t button ,u8_t* pu8_value)
{
    if (button == '1')
    {
        *pu8_value=num1;
    }
    else if (button == '2')
    {
        *pu8_value=num2;
    }
    else if (button == '3')
    {
        *pu8_value =num3;
    }
     else if (button == '4')
    {
        *pu8_value=num4;
    }
    else if (button == '5')
    {
        *pu8_value =num5;
    }
    
    else if (button == '6')
    {
        *pu8_value=num6;
    }
    else if (button == '7')
    {
        *pu8_value =num7;
    }
     else if (button == '8')
    {
        *pu8_value=num8;
    }
    else if (button == '9')
    {
        *pu8_value =num9;
    }
      else if (button == '0')
    {
        *pu8_value=num0;
    }
    else{

    }
    return;
    
    
    
}