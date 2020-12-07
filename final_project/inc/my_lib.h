#ifndef MY_LIB_H
#define MY_LIB_H

//#include <unistd.h>
#include <linux/delay.h>
#include "rpi_regs.h"

//
// GLOVAL VAR
//
extern int			fd_mem;
extern pGPIO_REGS	pGpio;
extern pPWM_REGS	pPwm;
extern pCLK_REGS	pClk;

//
// PROTOTYPE
//
void gpio_pinMode(int min, int mode);
void gpio_digitalWrite(int pin, int value);
void clk0_setDefault(void);
void clk1_setDefault(void);
void pwm1_setRange(int range);
void pwm1_setData(int data);
void pwm2_setRange(int range);
void pwm2_setData(int data);

//
// GPIO FUNC
//
void gpio_pinMode(int pin, int mode) {
  int i 		=  pin / 10; 		// 17일 경우 1번 레지스터(10~19)
  int bitOffset	= (pin % 10) * 3;  	// 17일 경우 7 * 3 = 21
 
  uint32_t oldGpfsel 	= pGpio->GPFSEL[i];
  uint32_t mask 		= 0b111 << bitOffset;
 
  // 비트클리어 후, OR 연산으로 i번째 GPFSEL를 mode로 변환
  pGpio->GPFSEL[i] = (oldGpfsel & ~mask) | ((mode << bitOffset) & mask); 
}

void gpio_digitalWrite(int pin, int value) {
  if (value)
    pGpio->GPSET[pin / 32] = ( 1 << (pin % 32) ); // 1이 들어오면  
  else
    pGpio->GPCLR[pin / 32] = ( 1 << (pin % 32) ); // 0이 들어오면
}

//
// CLOCK FUNC
//
void clk0_setDefault(void) {
	pClk->CM_GP0CTL = (0x5a << 24) | (1 << 5);
	while ( (pClk->CM_GP0CTL & (1 << 7)) != 0 );
	pClk->CM_GP0DIV = (0x5a << 24) | (19 << 12);
	pClk->CM_GP0CTL = (0x5a << 24) | (1 << 4) | (1 << 0);
}

void clk1_setDefault(void) {
	pClk->CM_GP1CTL = (0x5a << 24) | (1 << 5);
	while ( (pClk->CM_GP1CTL & (1 << 7)) != 0 );
	pClk->CM_GP1DIV = (0x5a << 24) | (19 << 12);
	pClk->CM_GP1CTL = (0x5a << 24) | (1 << 4) | (1 << 0);
}

//
// PWM_FUNC
//
void pwm1_setRange(int range) {
	pPwm->CTL &= ~(0xFF);
	udelay(1);
	pPwm->RNG1 = range - 1;
	pPwm->CTL |= 0x81;
}

void pwm1_setData(int data) {
	pPwm->CTL &= ~(0xFF);
	udelay(1);
	pPwm->DAT1 = data;
	pPwm->CTL |= 0x81;
}

void pwm2_setRange(int range) {
	pPwm->CTL &= ~(0xFF << 8);
	udelay(1);
	pPwm->RNG2 = range - 1;
	pPwm->CTL |= 0x81 << 8;
}

void pwm2_setData(int data) {
	pPwm->CTL &= ~(0xFF << 8);
	udelay(1);
	pPwm->DAT2 = data;
	pPwm->CTL |= 0x81 << 8;
}


#endif