//________________________________________________________________________________________________________________________________________________________________________
//
//																		TRAFFIC COUNTER PROJECT
//________________________________________________________________________________________________________________________________________________________________________



#ifndef __MAIN_H__
#define	__MAIN_H__

/*macros*/
#define LED1    PORTD_Bit6
#define LED2    PORTD_Bit7

#define I1      PIND_Bit3
#define I2      PIND_Bit2

#define LCD     PORTB

#define EN      PORTA_Bit7
#define RW      PORTA_Bit6
#define RS      PORTA_Bit5

#define K2      PINA_Bit0
#define K3      PINA_Bit1

#define CLK     3579545

/*Initializing functions*/
void lcd_home(void);
void system_init(void);
void lcd_init(void);
void lcd_en(void);
void timer_init(void);
void uart_init(void);

/*writing characters into the array*/
void write_char(unsigned int c);
/*writing string to array*/
void write_str(unsigned char* str);


unsigned char received_char(void);
void transmit_char(unsigned char c);

void write_num_on_lcd(void);


/*delays*/
void d_1us(void);
void d_50us(void);
void d_100us(void);
void d_1ms(void);
void d_5ms(void);
void d_10ms(void);
void d_50ms(void);
void d_100ms(void);
void d_1s(void);

#endif
