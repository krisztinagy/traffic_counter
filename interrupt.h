//________________________________________________________________________________________________________________________________________________________________________
//
//																		TRAFFIC COUNTER PROJECT
//________________________________________________________________________________________________________________________________________________________________________



#ifndef __INTERRUPT_H__
#define	__INTERRUPT_H__

extern unsigned char lcd[];
extern unsigned short ptr;

extern unsigned short sensor1_past;
extern unsigned short sensor1_present;

extern unsigned short sensor2_past;
extern unsigned short sensor2_present;

extern unsigned short flag1;
extern unsigned short flag2;

extern unsigned int timer1;
extern unsigned int timer2;

extern unsigned short button1_past;
extern unsigned short button1_present;
extern unsigned short button2_past;
extern unsigned short button2_present;

extern unsigned short flag_button1;
extern unsigned short flag_button2;

#endif
