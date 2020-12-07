#ifndef	RPI_REGS_H
#define RPI_REGS_H


#define RPI_GPIO	0x3F200000
#define RPI_PWM		0x3F20C000
#define RPI_CLK		0x3F101000

#define INPUT		0b000
#define OUTPUT		0b001
#define ALTF0		0b100
#define ALTF5		0b010

#define HIGH		1
#define LOW			0

#define DEG_0		600
#define DEG_90		1500
#define DEG_180		2500

//
// STRUCT
//
typedef struct TAG_GPIO_REGS {
	volatile uint32_t GPFSEL[6];	// 내부 모듈 선택 레지스터		INPUT , OUTPUT, ...
	volatile uint32_t _0;
	volatile uint32_t GPSET[2];  	// 핀 아웃풋 셋 레지스터		1=VCC / 0 = no effect
	volatile uint32_t _1;
	volatile uint32_t GPCLR[2];  	// 핀 아웃풋 클리어 레지스터  	1=GND / 0=no effect
	volatile uint32_t _2;
	volatile uint32_t GPCLEV[2];
	volatile uint32_t _3;
	volatile uint32_t GPEDS[2];
	volatile uint32_t _4;
	volatile uint32_t GPREN[2];
	volatile uint32_t _5;
	volatile uint32_t GPFEN[2];
	volatile uint32_t _6;
	volatile uint32_t GPHEN[2];
	volatile uint32_t _7;
	volatile uint32_t GPLEN[2];
	volatile uint32_t _8;
	volatile uint32_t GPAREN[0];
	volatile uint32_t _9;
	volatile uint32_t GPAFEN[2];
	volatile uint32_t _10;
	volatile uint32_t GPPID0;
	volatile uint32_t GPPUDCLK[2];
	volatile uint32_t _11;
	volatile uint32_t _12;
	volatile uint32_t _13;
	volatile uint32_t _14;
	volatile uint32_t _TEST;
} GPIO_REGS, *pGPIO_REGS;

typedef struct TAG_PWM_REGS {
	volatile uint32_t CTL;
	volatile uint32_t STA;
	volatile uint32_t DMAC;
	volatile uint32_t _0;
	volatile uint32_t RNG1;
	volatile uint32_t DAT1;
	volatile uint32_t FIF1;
	volatile uint32_t _1;
	volatile uint32_t RNG2;
	volatile uint32_t DAT2;
} PWM_REGS, *pPWM_REGS;

typedef struct TAG_CLK_REGS {
	union { volatile uint32_t _0[10][4]; };
	volatile uint32_t CM_GP0CTL;
	volatile uint32_t CM_GP0DIV;
	volatile uint32_t CM_GP1CTL;
	volatile uint32_t CM_GP1DIV;
	volatile uint32_t CM_GP2CTL;
	volatile uint32_t CM_GP2DIV;
} CLK_REGS, *pCLK_REGS;


#endif