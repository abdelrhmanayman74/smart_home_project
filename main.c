#include "LSTD_TYPES.h"
#include "LBIT_MATH.h"

#include "INEEPROM_interface.h"
#include "HLED_interface.h"
#include "HLCD_interface.h"
#include "MDIO_interface.h"
#include "MTIMER_interface.h"
#include <util/delay.h>
#include "MADC_interface.h"
#include<string.h>
#include "MUART_private.h"
#include "MUART_interface.h"
#include "HKPD_interface.h"


u16_t frq50hz = 5000;
u16_t Over_flow_to_open = 250;
u16_t Over_flow_to_close = 500;
s16_t duty_cycle = 0;
u8_t flag = 1;
u8_t x = 3;
u8_t flag2 = 1;
u8_t text[20];
u8_t n = 0;
u8_t  display_loop = 0;
u8_t action[6] = {0,0,0,0,0,0};
u16_t voltage;
u16_t temprature;
u16_t loc = 0;    // to know what location we write data in memory


void get_last_user_saved_in_memory(void);
void get_last_user_saved_in_memory(void)
{
    u16_t last_index_in_mem = 1023;
    u16_t get_user;
    while (last_index_in_mem > 0)
    {
        ineeprom_read(last_index_in_mem, &get_user);
        if (get_user == '+' && last_index_in_mem == 1023)
        {
            return;
        }

        else if (get_user == '+')
        {
            loc = last_index_in_mem + 1;
            return;
        }
        else
        {
            last_index_in_mem = last_index_in_mem - 1;
        }
        if (last_index_in_mem == 0)
        {
            loc = 0;
            return;
        }
        
        
    }
    
}




void get_temprature(u16_t* temp_reading);
void get_temprature(u16_t* temp_reading)
{
    madc_singleConversion(ADC_CHANNEL_1, &voltage);

    *temp_reading = (voltage*100)*5/1024;
    *temp_reading = *temp_reading - 2; 
   
    if(*temp_reading >= 28)
    {
        mdio_setPinValue(PORTC,PIN0,HIGH); // open air condition
        action[5] = 1;
            
        
        
    }
    else if(*temp_reading < 21)
    {
        mdio_setPinValue(PORTC,PIN0,LOW); // close air condition
            
        
        action[5] = 0;
    }
   
    else
    {

    }
    

}



void display_action_happened(void);
void display_action_happened(void)
{
    if (action[0] == 1 && display_loop == 0)
    {
        hlcd_displayString((u8_t*)"LED0 ON");
        _delay_ms(500);
        clear();
        _delay_ms(200);
    }

    if (action[1] == 1 && display_loop == 1)
    {
        hlcd_displayString((u8_t*)"LED1 ON");
        _delay_ms(500);
        clear();
        _delay_ms(200);
    }

    if (action[2] == 1 && display_loop == 2)
    {
        hlcd_displayString((u8_t*)"LED2 ON");
        _delay_ms(500);
        clear();
        _delay_ms(200);
    }
    if (action[3] == 1 && display_loop == 3)
    {
        hlcd_displayString((u8_t*)"DOOR OPENED");
        _delay_ms(500);
        clear();
        _delay_ms(200);
    }
    if (action[4] == 1 && display_loop == 4)
    {
        hlcd_displayString((u8_t*)"LAMP ON");
        _delay_ms(500);
        clear();
        _delay_ms(200);
    }
    if (action[5] == 1 && display_loop == 5)
    {
        hlcd_displayString((u8_t*)"AIR CONDITION ON");
        _delay_ms(500);
        clear();
        _delay_ms(200);
    }
    display_loop = display_loop + 1;

    if(display_loop > 5)
    {
        display_loop = 0;

    }

}



void filter();
void filter()
{
    if (flag2 == 1)
    {
        if (x==0)
        {
            n = 0;
            flag2 = 0;
        }
        
        x = x-1;
    }
    
}


void display_recvByte(u8_t* pu8_dataByte);
void display_recvByte(u8_t* pu8_dataByte)
{
    /*Checking if there's a new data received or not*/
    
    while(GET_BIT(MUART_UCSRA, MUART_UCSRA_RXC_BIT) == 1)
    {
    
        
        /*Getting the new received data*/
        *pu8_dataByte = MUART_UDR;
        text[n] = *pu8_dataByte;
        
        n = n + 1;
      
    }

    /*Return from this function*/
    return;
}


void dimming_lamp_inc(void);
void dimming_lamp_inc(void)
{
    duty_cycle = 15 + duty_cycle * 2;
    if (duty_cycle >= 255)
    {
        duty_cycle = 255;
    }
    
    
    if (flag == 1)
    {
        mtimer_startPWM_signal(TIMER_CHANNEL_2);
        mtimer_runPWM_signal(TIMER_CHANNEL_2,duty_cycle);
        _delay_ms(1000);
        flag = 0;

    }
    else
    {
        mtimer_runPWM_signal(TIMER_CHANNEL_2,duty_cycle);
        _delay_ms(1000);

    }

}

void dimming_lamp_dec(void);
void dimming_lamp_dec(void)
{

    duty_cycle -= 64;
    if (duty_cycle <= 0)
    {
        
        duty_cycle = 0;
        mtimer_stopPWM_signal(TIMER_CHANNEL_2);
        flag = 1;
        
    }
    else
    {
        mtimer_runPWM_signal(TIMER_CHANNEL_2,duty_cycle);
        _delay_ms(1000);

    }
    
}


void OpenDoor(void);
void OpenDoor(void)
{
    
    GenerateDesierdFrequency(frq50hz, Over_flow_to_open);
    
    
}

void CloseDoor(void);
void CloseDoor(void)
{
    
    GenerateDesierdFrequency(frq50hz, Over_flow_to_close);
    
}


int main(void)
{
    u8_t au8data;
    
    u8_t button = 'f';
    u8_t sign_in_happened = 0;
    u8_t door_opened = 0;
    u8_t check_reset = 3;
    u8_t admin_permission = 0;
    u8_t remoted_reset = 3;
    u8_t is_signed_in = 0;


    mdio_setPinStatus(PORTD,PIN0,INPUT_FLOAT);  // for bluetooth module
     
    

    muart_init(UART_9600_BPS);
    hled_init(LED2);
    hled_init(LED1);
    hled_init(LED0);
    hkpd_init();
    hlcd_init();
    madc_init(AVCC_MODE, ADC_10_BITS,ADC_PRESCALER_16);
    
    
    

    mdio_setPinStatus(PORTD,PIN5,OUTPUT); //GENERATE PWM FOR SERVO MOTOR

    mdio_setPinStatus(PORTB,PIN5,OUTPUT); // ground for temp sensor
    mdio_setPinValue(PORTB,PIN5,LOW);

    mdio_setPinStatus(PORTB,PIN6,OUTPUT); // 5 volt for temp sensor
    mdio_setPinValue(PORTB,PIN6,HIGH);

    mdio_setPinStatus(PORTA,PIN1,INPUT_FLOAT); // sensor reading
    
    mdio_setPinStatus(PORTC,PIN0,OUTPUT); // DC MOTOR
    


    
    mdio_setPinStatus(PORTD,PIN7,OUTPUT);   //GENERATE PWM FOR LED

    mtimer_init(TIMER_CHANNEL_2, TIMER_PWM_MODE, TIMER_FREQ_1KHZ);

    get_last_user_saved_in_memory(); 
    
    hlcd_displayString((u8_t*)"SMART HOME");
    _delay_ms(3000);
    clear();
                
    while (1)
    {
        button = 'f';
        display_recvByte(&au8data);
        filter();
        
        get_temprature(&temprature);

        if (text[0] == 'C')
        {
           
            remoted_reset = 3;
            text[0] = '0';
            n = 0;
            
        }

        if (text[0] == 'F')
        {
            remoted_reset = remoted_reset - 1;
            text[0] = '0';
            n = 0;
            if (remoted_reset == 0)
            {
                hlcd_displayString((u8_t*)"PLEASE RESET");
                while (1)
                {
                    /* code */
                }
                
            }
            
            

        }

        
        if (text[0] == 'P')
        {
            admin_permission = 1;
           
            text[0] = '0';
            n = 0;
        }

        if (text[0] == 'N')
        {
            admin_permission = 0;
            
            text[0] = '0';
            n = 0;
            

        }

        if (text[0] == '1')
        {
            hled_ledValueON(LED0);
            text[0] = '0';
            n = 0;
            action[0] = 1;

        }
        else if (text[0] == '2')
        {
            hled_ledValueON(LED1);
            text[0] = '0';
            n = 0;
            action[1] = 1;

        }
        else if (text[0] == '3')
        {
            hled_ledValueON(LED2);
            text[0] = '0';
            n = 0;
            action[2] = 1;

        }

        else if (text[0] == '4')
        {
            hled_ledValueOFF(LED0);
            text[0] = '0';
            n = 0;
            action[0] = 0;

        }
        else if (text[0] == '5')
        {
            hled_ledValueOFF(LED1);
            text[0] = '0';
            n = 0;
            action[1] = 0;

        }
        else if (text[0] == '6')
        {
            hled_ledValueOFF(LED2);
            text[0] = '0';
            n = 0;
            action[2] = 0;

        }
        else if (text[0] == 'W')
        {
            mdio_setPinValue(PORTC,PIN0,HIGH); // open air condition

            text[0] = '0';
            n = 0;
            action[5] = 1;

        }

        else if (text[0] == 'I')
        {
            mdio_setPinValue(PORTC,PIN0,LOW); // close air condition

            text[0] = '0';
            n = 0;
            action[5] = 0;

        }


        else if (text[0] == '7' && door_opened == 0)
        {
            OpenDoor();
            door_opened = 1;
            text[0] = '0';
            n = 0;
            action[3] = 1;
            

        }
        else if (text[0] == '8' && door_opened == 1)
        {
            CloseDoor();
            door_opened = 0;
            text[0] = '0';
            n = 0;
            action[3] = 0;

        }

        else if (text[0] == '*')
        {
            dimming_lamp_inc();
            text[0] = '0';
            n = 0;

        }

        else if (text[0] == '/')
        {
            dimming_lamp_dec();
            text[0] = '0';
            n = 0;

        }

        else if (text[0] == 'D' && au8data == 'E')
        {
            u16_t start_del;
            u16_t end_del;
            u8_t res = 0;;
            u8_t del[15];
            u8_t num = 0;
            u8_t index  = 0;
            u8_t start = 0;
            u16_t read;
            u8_t user_size[n];
            user_size[index] = '-';
            index = index + 1;
            for (u8_t i = 1; i < n-1; i++)
            {
                user_size[i] = text[i];
                index = index + 1;

            }
            user_size[n-1] = '+';
            index = index + 1;
            text[0] = '0';
            n = 0;
            for (u16_t j = 0; j < 1022; j++)
            {
                ineeprom_read(j, &read);
                _delay_ms(10);
                if (read == '-' && start == 0)
                {
                    start = 1;
                    num = 0;
                    start_del = j;
                }
                if (start == 1)
                {
                    del[num] = read;
                    
                    num = num + 1;
                    
                }
                if (read == '+' && start == 1)
                {
                    start = 0;
                    end_del = j;
                    if (num == index)
                    {
                        res = 1;
                        for (u8_t k = 0; k < index; k++)
                        {
                            if (del[k] != user_size[k])
                            {
                                res = 0;

                            }

                        }
                        if (res == 1)
                        {
                            sign_in_happened  = 0;
                            is_signed_in = 0;
                            for (u16_t k = start_del; k < end_del+1; k++)
                            {
                                ineeprom_write(k, 0xFF);
                                _delay_ms(10);
                                
                            }
                        }
                        
                    }
                    else 
                    {
                        num = 0;
                    }

                }
             
            }
        
        }

        
        else if (text[0] == '-' && au8data == '+')
        {
             
            for (u8_t i = 0; i < n; i++)
            {
                if (loc < 1020)
                {
                    ineeprom_write(loc, text[i]);
                    _delay_ms(10);
                    loc = loc + 1;
                }
                
                
            }
            text[0] = '0';
            n = 0;
            
                  
        }
        else
        {
                    // do nothing
        }


                

        hkpd_getPressedButton(&button);
        if (sign_in_happened == 1)
        {
            if (button == '1' && admin_permission == 1)
            {
            hled_ledValueON(LED0);
            action[0] = 1;
            }
            else if (button == '1' && admin_permission == 0)
            {
                hlcd_displayString((u8_t*)"NO PERMISSION");
                _delay_ms(2000);
                clear();
                _delay_ms(200);
            }
            

            else if (button == '2' && admin_permission == 1)
            {
            hled_ledValueON(LED1);
            action[1] = 1;
            }
            else if (button == '2' && admin_permission == 0)
            {
                hlcd_displayString((u8_t*)"NO PERMISSION");
                _delay_ms(2000);
                clear();
                _delay_ms(200);
            }

            else if (button == '3' && admin_permission == 1)
            {
            hled_ledValueON(LED2);
            action[2] = 1;
            }
            else if (button == '3' && admin_permission == 0)
            {
                hlcd_displayString((u8_t*)"NO PERMISSION");
                _delay_ms(2000);
                clear();
                _delay_ms(200);
            }
            

            else if (button == '4' && admin_permission == 1)
            {
            hled_ledValueOFF(LED0);
            action[0] = 0;
           
            }
            else if (button == '4' && admin_permission == 0)
            {
                hlcd_displayString((u8_t*)"NO PERMISSION");
                _delay_ms(2000);
                clear();
                _delay_ms(200);
            }

            else if (button == '5' && admin_permission == 1)
            {
            hled_ledValueOFF(LED1);
            action[1] = 0;
           
            }
            else if (button == '5' && admin_permission == 0 )
            {
                hlcd_displayString((u8_t*)"NO PERMISSION");
                _delay_ms(2000);
                clear();
                _delay_ms(200);
            }

            else if (button == '6' && admin_permission == 1)
            {
            hled_ledValueOFF(LED2);
            action[2] = 0;
           
            }
            else if (button == '6' && admin_permission == 0)
            {
                hlcd_displayString((u8_t*)"NO PERMISSION");
                _delay_ms(2000);
                clear();
                _delay_ms(200);
            }

            else if (button == '7' && admin_permission == 1)
            {
            
            mdio_setPinValue(PORTC,PIN0,HIGH); // open air condition
            
            
            action[5] = 1;
            
           
            }
            else if (button == '7' && admin_permission == 0)
            {
                hlcd_displayString((u8_t*)"NO PERMISSION");
                _delay_ms(2000);
                clear();
                _delay_ms(200);
            }
            else if (button == '8' && admin_permission == 1)
            {
            // DC MOTOR
            mdio_setPinValue(PORTC,PIN0,LOW);   // close air condition
            
            
            action[5] = 0;
            
           
            }
            else if (button == '8' && admin_permission == 0)
            {
                hlcd_displayString((u8_t*)"NO PERMISSION");
                _delay_ms(2000);
                clear();
                _delay_ms(200);
            }

            else if (button == '9' && admin_permission == 1)
            {
            dimming_lamp_inc();
            hlcd_displayString((u8_t*)"Lamp inc");
            _delay_ms(2000);
            clear();
           
            }
            else if (button == '9' && admin_permission == 0)
            {
                hlcd_displayString((u8_t*)"NO PERMISSION");
                _delay_ms(2000);
                clear();
                _delay_ms(200);
            }
            else if (button == '0' && admin_permission == 1)
            {
            dimming_lamp_dec();
            hlcd_displayString((u8_t*)"Lamp dec");
            _delay_ms(2000);
            clear();
           
            }
            else if (button == '0' && admin_permission == 0)
            {
                hlcd_displayString((u8_t*)"NO PERMISSION");
                _delay_ms(2000);
                clear();
                _delay_ms(200);
            }
            else
            {

            }
            
        }
        if (button == 'R' && is_signed_in == 0)
        {
            hlcd_displayString((u8_t*)"ENTER USERNAME");
            hlcd_displayPosition(LCD_ROW_1, LCD_COL_0);
            _delay_ms(2000);
            u8_t name;
            u8_t index = 0;
            u8_t check_na_pass[20];
            check_na_pass[index] = '-';
            index = index + 1;
            u8_t z = 1;
            while (z == 1)
            {
                hkpd_getPressedButton(&name);
                if(name == 'A' || name == 'B' || name == 'C' || name == 'D')
                {
            
                    check_na_pass[index] = name;
                    hlcd_displayCharacter(check_na_pass[index]);
                    _delay_ms(700);
                    index = index + 1;
                }
                if (name == 'R')
                {
                    z = 0;
                }
                
            }
            clear();
            _delay_ms(1000);
            hlcd_displayString((u8_t*)"ENTER PASSWORD");

            hlcd_displayPosition(LCD_ROW_1, LCD_COL_0);
            _delay_ms(2000);
            z = 1;
            while (z == 1)
            {
                hkpd_getPressedButton(&name);
                if(name == '1' || name == '2' || name == '3' || name == '4' || name == '5' || name == '6' || name == '7' || name == '8' || name == '9')
                {
                    
                    check_na_pass[index] = name;
                    hlcd_displayCharacter(check_na_pass[index]);
                    _delay_ms(700);
                    index = index + 1;
                }
                if (name == 'R')
                {
                    z = 0;
                }
            }
            check_na_pass[index] = '+';
            index = index + 1;
            clear();
            _delay_ms(1000);
            hlcd_displayString((u8_t*)"CHECKING ...");
            _delay_ms(2000);
            clear();
            _delay_ms(1000);
            u8_t test[10];
            u8_t res = 0;
            u16_t get_value;
            
            u8_t start = 0;
            u16_t num = 0;
            

            
            for (u16_t i = 0; i < 15; i++)
            {

                ineeprom_read(i, &get_value);
                
                if (get_value == '-' && start == 0)
                {
                    
                    start = 1;
                    num = 0;
                    
                    
                }
                if (start == 1)
                {
                    test[num] = get_value;
                    num = num +1;
                    
                    
                }
                if (get_value == '+' && start == 1)
                {
                    start = 0;
                    
                    

                    if (num == index)
                    {
                        
                        res = 1;
                        
                        for (u8_t z = 0; z < num; z++)
                        {
                            
                            if (test[z] != check_na_pass[z])
                            {
                                res = 0;
                            }
                        }
                        
                        if (res == 1)
                        {
                            is_signed_in = 1;
                            hlcd_displayString((u8_t*)"Successful login");
                            _delay_ms(2000);
                            sign_in_happened = 1;
                            clear();
                            _delay_ms(200); 
                            check_reset = 3;
                            break;
                        }
                        else
                        {

                        }   
                        
                        
                    }
                    else
                    {
                        num = 0;
                    }       
                    
                }
            
            }
            if (res == 0)
            {
                
                hlcd_displayString((u8_t*)"SIGN IN FAILED");
                _delay_ms(2000);
                clear();
                _delay_ms(1000);
                check_reset = check_reset - 1;
            }
            if (check_reset == 0)
            {
                hled_ledValueON(LED0);
                hlcd_displayString((u8_t*)"PLEASE RESET");
                while (1)
                {
                    /* code */
                }
                
            }
          
            
        }
            
        if (button == 'E' && is_signed_in == 1)
        {
            is_signed_in = 0;
            sign_in_happened = 0;
            hlcd_displayString((u8_t*)"LOGGING OUT...");
            _delay_ms(2000);
            clear();
            _delay_ms(1000);

        }
        display_action_happened();
        
    }

    
    return 0;
   
}