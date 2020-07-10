/*
 * GccApplication1.c
 *
 * Created: 2020/3/28 20:23:05
 * Author : Administrator
 */ 
#define F_CPU 4000000
#define _BM(bit) (uint8_t)(1U<<bit)
#include <avr/io.h>
#include <util/delay.h>
int main(void)
{
	MCUCR = 0b10000001;//程序从flash区运行，中断向量指向flash区
	MCUCR |= (1<<ENPS);//使能端口设定
	PORTD=0xff;
	DDRD =0x05; //PD2,LED端口，设为输出
    while (1) 
    {
		_delay_ms(500);	
		PORTD ^= _BM(2);
	}
}

