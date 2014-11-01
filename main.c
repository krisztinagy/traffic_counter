//________________________________________________________________________________________________________________________________________________________________________
//
//																		TRAFFIC COUNTER PROJECT
//________________________________________________________________________________________________________________________________________________________________________



#define ENABLE_BIT_DEFINITIONS

#include <ioavr.h>
#include <inavr.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "main.h"
#include "binary.h"

unsigned char lcd[81];
unsigned short ptr_lcd;
unsigned short ptr;

/*Previous and current states of the gates*/
unsigned short sensor1_past;
unsigned short sensor1_present;

unsigned short sensor2_past;
unsigned short sensor2_present;

unsigned short flag1;
unsigned short flag2;

unsigned int timer1;
unsigned int timer2;

/*Previous and current states of the buttons*/
unsigned short button1_past;
unsigned short button1_present;

unsigned short button2_past;
unsigned short button2_present;

unsigned short flag_button1;
unsigned short flag_button2;

/*Counters for crossings*/
unsigned int people_in;
unsigned int people_all;

/*Data strings*/
unsigned int num;
unsigned int in;
unsigned int all;

/*Defining delays*/
void d_1us(void)   { __delay_cycles(CLK/1000000); } //1 usec
void d_50us(void)  { __delay_cycles(CLK/20000); }   //50 usec
void d_100us(void) { __delay_cycles(CLK/10000); }   //100 usec
void d_500us(void) { __delay_cycles(CLK/2000); }    //500 usec
void d_1ms(void)   { __delay_cycles(CLK/1000); }    //1 msec
void d_5ms(void)   { __delay_cycles(CLK/200); }     //5 msec
void d_10ms(void)  { __delay_cycles(CLK/100); }     //10 msec
void d_50ms(void)  { __delay_cycles(CLK/20); }      //50 msec
void d_100ms(void) { __delay_cycles(CLK/10); }      //100 msec
void d_1s(void)    { __delay_cycles(CLK); }         //1 sec

/*LCD Initialization*/
void lcd_init(void)
{  
    LCD=b00111100;
    RS=0;
    RW=0;
    lcd_en();
    
    d_50us();
    
    LCD=b00001100;
    RS=0;
    RW=0;
    lcd_en();
    
    d_50us();
    
    LCD=b00000001;
    RS=0;
    RW=0;
    lcd_en();
    
    d_5ms();
    
    LCD=b00000110;
    RS=0;
    RW=0;
    lcd_en();
    
    d_50us();
}

void lcd_en(void)
{
    EN=1;
    d_1us();
    EN=0;
}


void lcd_home(void)
{
    RS=0;
    RW=0;
    LCD=0x03;
    lcd_en();
    d_50us();
}

/*System initialization*/
void system_init(void)
{
    /*PORTX*/
    /*output: dont care*/
    /*input: if 	0: no pull up resistor,
                    1: pull up resistor */
    
    /*DDRX*/
    /*directions: 0:input 1:output*/
    
    
    PORTA=b00000011;    DDRA=b11111100;
    PORTB=b00000000;    DDRB=b11111111;
    PORTD=b00000000;    DDRD=b11110011;
   
    EN=0;
    RW=0;
    RS=0;
    LED2=0;
    LED1=0;
    
    /*Setting LCD array and pointer to null*/
    ptr_lcd=0;
    ptr=0;
    memset(lcd, ' ', sizeof(lcd));
    lcd[80] = '\0';
    
    
    sensor1_past=0;
    sensor1_present=0;
    sensor2_past=0;
    sensor2_present=0;
    
    flag1=0;
    flag2=0;
    timer1=0;
    timer2=0;
    
    button1_past=0;
    button1_present=0;
    button2_past=0;
    button2_present=0;
    
    flag_button1=0;
    flag_button2=0;
    
    people_in=0;
    people_all=0;
}

/*Initializing timer*/
void timer_init(void)
{
    /*Normal mode*/
    TCCR0A=b00000000;
    /*Set prescaling=64*/
    TCCR0B=b00000011;
    /*Enable overflow interrupt*/
    TIMSK0=b00000001;
    /*Set start value of counter*/
    TCNT0=0xC8;
    
}

void uart_init(void)
{
    /*Set everything to null*/
    UCSR0A=0x00;
    /*Enable TX RX*/
    UCSR0B=b00011000;
    /*asynchron, no parity, 1 stop, 8 data bits
    sending to rising edge, sampling to falling edge */
    UCSR0C=b00000110;
    
    /*9600 baud rate: 1.3% error at 3.5MHz*/
    UBRR0H=0x00;
    UBRR0L=0x16;  
}

unsigned char received_char(void)
{
	if((UCSR0A & (1<<RXC0)))
		return UDR0; 
	else
		return 0;
}

void transmit_char(unsigned char c)
{
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = c;
}

void transmit_str(unsigned char* str)
{
    int i=0;
    while(str[i] != '\0')
    {
       transmit_char(str[i]);
        
       i++;
    }
}

void write_char(unsigned int c)
{
    if(ptr_lcd > 79) { ptr_lcd = 0; }
    
    lcd[ptr_lcd]=c;
    
    switch(ptr_lcd)
    {
        case 19:
            ptr_lcd=40;
            break;
        case 59:
            ptr_lcd=20;
            break;
        case 39:
            ptr_lcd=60;
            break;
        case 79:
            ptr_lcd=0;
            break;
        default:
            ptr_lcd++;
            break;
    }
}

void write_str(unsigned char* str)
{
    int i=0;
    while(str[i] != '\0')
    {
       write_char(str[i]);
        
       i++;
    }
}

void write_num_on_lcd(void)
{
    int i;
    int j;
    
    in=people_in;
    all=people_all;
    
    ptr_lcd=19;
    i=0;
    for(i=0; i<4; i++)
    {
        num=in%10;
        write_char((char)(num+48));
        in=(unsigned int)in/10;
        ptr_lcd=18-i;
    }
    
    ptr_lcd=39;
    j=0;
    for(j=0; j<4; j++)
    {
        num=all%10;
        write_char((char)(num+48));
        all=(unsigned int)all/10;
        ptr_lcd=38-j;
    }
}

int main( void )
{
    
    system_init();
    lcd_init();
    uart_init();
    timer_init();
    
    DDRA_Bit3=1;
    PORTA_Bit3=0;
    
    __enable_interrupt(); 
    
	write_str("  Traffic counter                       Nagy Krisztina 2013");
    d_1s();
    d_1s();
    d_1s();   
    
    ptr_lcd=0;
	write_str("People IN:          ");
    write_str("                    ");
	write_str("Total traffic:      ");
    write_str("                    ");
    
    while(1)
    {
        if(flag1==1)
        {
            timer1=1000;
            flag1=0;
        }
        if(flag2==1)
        {
            timer2=1000;
            flag2=0;
        }
   
        if((timer1>0 && timer2>0) && (timer2 > timer1))
        {
            people_in++;
            people_all++;
            flag1=0;
            flag2=0;
            timer1=0;
            timer2=0;
            
            /*write to LCD*/
            write_num_on_lcd();
                
            
        }
        if((timer1>0 && timer2>0) && (timer1 > timer2))
        {
            if(people_in)
            {
                people_in--;
            }
            flag1=0;
            flag2=0;
            timer1=0;
            timer2=0;
            
            /*write to LCD*/
            write_num_on_lcd();
        }
        
        if(flag_button1==1)
        {
            people_in=0;
            write_num_on_lcd();
            flag_button1=0;
        }
        if(flag_button2==1)
        {
            transmit_str("Number of people in the room:");
            transmit_char(lcd[16]);
            transmit_char(lcd[17]);
            transmit_char(lcd[18]);
            transmit_char(lcd[19]);
            transmit_char((char)13);
            
            transmit_str("Total traffic:");
            transmit_char(lcd[36]);
            transmit_char(lcd[37]);
            transmit_char(lcd[38]);
            transmit_char(lcd[39]);
            transmit_char((char)13);
            
            flag_button2=0;
        } 
    }
    
}
