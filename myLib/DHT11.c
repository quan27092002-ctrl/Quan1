#include "DHT11.h"


void delay_us(uint16_t time){
	HAL_TIM_Base_Start(&htim2);
	htim2.Instance->CNT = 0;
	while (htim2.Instance->CNT <time){};
		HAL_TIM_Base_Stop(&htim2);
}

void setToOutput(void){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
void setToInput(void){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void DHT11_Start(void){
	setToOutput();
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);
	HAL_Delay(20);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);
	setToInput();
	//function check loi
	while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2)==GPIO_PIN_SET){};	
	while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2)==GPIO_PIN_RESET){};	
	while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2)==GPIO_PIN_SET){};	
	//bat dau send data 40 bits
}
uint8_t DHT11_Read(void){
	uint8_t data;
	for(uint8_t i=0;i<8;i++ ){
		while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2)==GPIO_PIN_RESET){};
		delay_us(50);//delay us
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2)==GPIO_PIN_SET){
			//bit 1
			data = (data<<1)|(1<<0);
		}
		else{
			//bit0
			data = (data<<1)&~(1<<0);
		}
		while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2)==GPIO_PIN_SET){};
	}
	return data;
}



//void DHT11_Handler(void){
//	DHT11_Start();
//	uint8_t Hum_int = DHT11_Read();
//	uint8_t Hum_float = DHT11_Read();
//	uint8_t Tem_int = DHT11_Read();
//	uint8_t Tem_float = DHT11_Read();
//	uint8_t checkSum = DHT11_Read();

//}

