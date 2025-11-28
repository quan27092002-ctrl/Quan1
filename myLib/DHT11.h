#ifndef _DHT11_H_
#define _DHT11_H_

#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef htim2;


void delay_us(uint16_t time);
void setToOutput(void);
void setToInput(void);
void DHT11_Start(void);
uint8_t DHT11_Read(void);
//void DHT11_Handler(void);


#endif 
