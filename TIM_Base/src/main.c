/*
 * blinkLed.c
 *
 *  Created on: Sep 25, 2014
 *      Author: huyfly
 */
#include <stm32f4xx.h>

void init_LED(void);
void init_TIM(void);
void enable_TimerInterrupt(void);
int main() {
	init_LED();
	init_TIM();
	enable_TimerInterrupt();
	while (1) {
		for (int i = 0; i < 1000000; i++)
			asm("nop");
		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
	}
}
void init_LED() {
	GPIO_InitTypeDef GPIO_struct;

	/*enable clock for GPIOD*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/*Configure PIN mode*/
	GPIO_struct.GPIO_Pin =
	GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_struct.GPIO_Mode = GPIO_Mode_OUT;					//Pin12, Pin13, Pin14, Pin15 is output
	GPIO_struct.GPIO_Speed = GPIO_Fast_Speed;				//100Mhz
	GPIO_struct.GPIO_OType = GPIO_OType_PP;
	GPIO_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;

	/*Initialize GPIOD*/
	GPIO_Init(GPIOD, &GPIO_struct);
}

void init_TIM() {
	/*Enable Clock for TIMER 2*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/*Configure TIMER 2. TIM2 is counter mode*/
	TIM_TimeBaseInitTypeDef TIM_Struct;
	TIM_Struct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_Struct.TIM_Period = 500;
	TIM_Struct.TIM_Prescaler = 4000;
	TIM_Struct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_Struct.TIM_RepetitionCounter = 0;

	/*Initialize TIM2*/
	TIM_TimeBaseInit(TIM2, &TIM_Struct);

	/*Start TIM2*/
	TIM_Cmd(TIM2, ENABLE);

	/*Allow interrupt*/
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

void enable_TimerInterrupt() {
	NVIC_InitTypeDef NVIC_Struct;

	NVIC_Struct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_Struct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Struct.NVIC_IRQChannelSubPriority = 1;

	NVIC_Init(&NVIC_Struct);
}
