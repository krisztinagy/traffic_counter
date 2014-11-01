//________________________________________________________________________________________________________________________________________________________________________
//
//																		TRAFFIC COUNTER PROJECT
//________________________________________________________________________________________________________________________________________________________________________

#include "main.h"
#include "interrupt.h"

#include <ioavr.h>
#include <inavr.h>
#include <string.h>

#pragma vector=TIMER0_OVF_vect
__interrupt void TIMER0_OVF_handler(void)
{
    __disable_interrupt();    
    TCNT0=0xC8;
    PORTA_Bit3=1;

    /*display*/
    if(ptr>=80)
    {
        ptr=0;
        lcd_home();
    }
    else
    {
        RS=1;
        RW=0;
        LCD=lcd[ptr];
        lcd_en();
        ptr++;
    }
    
    /*Writing the characters from the buffer to the LCD screen*/
   unsigned char c=received_char();
   if(c!=(char)0)
   {
       transmit_char(c);
       c=(char)0;
   }
   
   /*Lighting up LEDs while there is an objects between the sensors*/
   if(I1)
       LED1=1;
   else
       LED1=0;
   
   if(I2)
       LED2=1;
   else
       LED2=0;
   
   /*Sampling sensors*/
   sensor1_past=sensor1_present;
   sensor1_present=I2;
   
   sensor2_past=sensor2_present;
   sensor2_present=I1;
   
   /*Setting flags*/
   if(sensor1_past==0 && sensor1_present==1)
       flag1=1;
   if(sensor1_past==1 && sensor1_present==0)
       flag1=0;
   
   if(sensor2_past==0 && sensor2_present==1)
       flag2=1;
   if(sensor2_past==1 && sensor2_present==0)
       flag2=0;

   /*Sampling pushbuttons*/
   button1_past=button1_present;
   if(!K2)
       button1_present = 1;
   else
       button1_present = 0;
   
   button2_past=button2_present;
   if(!K3)
       button2_present = 1;
   else
       button2_present = 0;
   
   /*Setting pushbutton flags*/
   if(button1_past==0 && button1_present==1)
       flag_button1=1;
   if(button1_past==1 && button1_present==0)
       flag_button1=0;
   
   if(button2_past==0 && button2_present==1)
       flag_button2=1;
   if(button2_past==1 && button2_present==0)
       flag_button2=0;
   
   /*Decrease timers*/
   if(timer1>0) { timer1--; }
   if(timer2>0) { timer2--; }
   
    PORTA_Bit3=0;
    __enable_interrupt();
}
