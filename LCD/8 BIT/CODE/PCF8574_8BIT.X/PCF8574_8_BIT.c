
// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define RS RC0
#define EN RC1
#define _XTAL_FREQ 4000000

void i2c_ins();
void i2c_st();
void i2c_restart();
void i2c_write(char);
char i2c_read();
void i2c_stop();
void lcd(char,char);
void puls();
void cmd(char,char,char);
void st(char*,char);

int main()
{
	PORTC=0X00;
	TRISC=0X3C;
	PORTA=0X00;
	TRISA=0X00;
	ANSEL=ANSELH=0X00;
	i2c_ins();
	
	cmd(0,0x0e,0x40);
	cmd(0,0x38,0x40);
	
	while(1)
	{
		cmd(0,0x80,0x40);
		if(RA0==1)
		{
			st("hai",0x40);
			while(RA0==1);
		}
		if(RA1==1)
		{
			st("bye",0x40);
			while(RA1==1);
		}
	}	
}
void i2c_ins()
{
	SSPSTAT=0X80;
	SSPCON=0X28;
	SSPCON2=0X00;
}
void i2c_st()
{
	SEN=1;
	while(SEN==1);
}
void i2c_write(char data)
{
	SSPBUF=data;
	while(BF==1);
	while(ACKSTAT==1);
}
void i2c_stop()
{
	PEN=1;
	while(PEN==1);
}
void lcd(char data,char add)
{
	i2c_st();
	i2c_write(add);
	i2c_write(data);
	i2c_stop();
}
void puls()
{
	EN=1;
	__delay_us(50);
	EN=0;
	__delay_us(50);
}
void cmd(char command,char data,char add)
{
	RS=command;
	lcd(data,add);
	puls();	
}
void st(char *data,char add)
{
	while(*data)
	cmd(1,*data++,add);
}