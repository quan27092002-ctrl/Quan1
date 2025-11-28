#include "TFT.h"



void WriteCmd(uint8_t cmd){
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET); //cs 0
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET); //a0=0 -> cmd
	HAL_SPI_Transmit(&hspi1,&cmd,SIZE_OF_CMD,1000);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET); // cs 1
}
void WriteData(uint8_t data){
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET); //a0=0 -> data
	HAL_SPI_Transmit(&hspi1,&data,SIZE_OF_DATA,1000);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
}


void setPos(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2){
	WriteCmd(0x2A);
	WriteData(0x00);
	WriteData(x1);
	WriteData(0x00);
	WriteData(x2);
	
	
  WriteCmd(0x2B);
	WriteData(0x00);
	WriteData(y1);
	WriteData(0x00);
	WriteData(y2);
}


void TFT_init(void){
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);
	HAL_Delay(20);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);
	HAL_Delay(200);
	// --- Software Reset ---
  WriteCmd(0x01); // SWRESET
  HAL_Delay(150); // >120ms

  // --- Sleep Out ---
  WriteCmd(0x11); // SLPOUT
  HAL_Delay(255); 

  // --- Frame Rate (L?m m?n h?nh d? rung) ---
  WriteCmd(0xB1); // FRMCTR1
  WriteData(0x01); WriteData(0x2C); WriteData(0x2D);
  WriteCmd(0xB2); // FRMCTR2
  WriteData(0x01); WriteData(0x2C); WriteData(0x2D);
  WriteCmd(0xB3); // FRMCTR3
  WriteData(0x01); WriteData(0x2C); WriteData(0x2D);
  WriteData(0x01); WriteData(0x2C); WriteData(0x2D);
	
	
	WriteCmd(0xB4);
	WriteData(0x07);

  // --- Power Sequence  ---
  WriteCmd(0xC0); // PWCTR1
  WriteData(0xA2); WriteData(0x02); WriteData(0x84);
  WriteCmd(0xC1); // PWCTR2
  WriteData(0xC5);
  WriteCmd(0xC2); // PWCTR3
  WriteData(0x0A); WriteData(0x00);
  WriteCmd(0xC3); // PWCTR4
  WriteData(0x8A); WriteData(0x2A);
  WriteCmd(0xC4); // PWCTR5
  WriteData(0x8A); WriteData(0xEE);
  WriteCmd(0xC5); // VMCTR1
  WriteData(0x0E);

  // --- Gamma Sequence  ---
  WriteCmd(0xE0);
  WriteData(0x02); WriteData(0x1C); WriteData(0x07); WriteData(0x12);
  WriteData(0x37); WriteData(0x32); WriteData(0x29); WriteData(0x2D);
  WriteData(0x29); WriteData(0x25); WriteData(0x2B); WriteData(0x39);
  WriteData(0x00); WriteData(0x01); WriteData(0x03); WriteData(0x10);

  WriteCmd(0xE1);
  WriteData(0x03); WriteData(0x1D); WriteData(0x07); WriteData(0x06);
  WriteData(0x2E); WriteData(0x2C); WriteData(0x29); WriteData(0x2D);
  WriteData(0x2E); WriteData(0x2E); WriteData(0x33); WriteData(0x3F);
  WriteData(0x00); WriteData(0x00); WriteData(0x02); WriteData(0x10);


	// --- Color Mode  ---
  WriteCmd(0x3A); // COLMOD: Pixel Format Set
  WriteData(0x05); // 16-bit/pixel (5-6-5 format)
	
	//Display inversion off
	WriteCmd(0x20);


  // --- Memory Access ---
  WriteCmd(0x36); // MADCTL
  WriteData(0x08); // MX, MY, RGB mode 0x08 ??? 
	
  // --- Display On ---
	setPos(0,0,WIDTH_OF_LCD,HEIGHT_OF_LCD);
  WriteCmd(0x29); // DISPON
  HAL_Delay(100);
	


}
void full_color(uint16_t color){
	WriteCmd(0x2C);
	for(int i =0;i <WIDTH_OF_LCD*HEIGHT_OF_LCD;i++){
		WriteData(color & 0xFF);
		WriteData(color >> 8);
	
	}

}
void drawPixel (uint8_t x,uint8_t y, uint8_t color ){
	if(x>=WIDTH_OF_LCD || y>=HEIGHT_OF_LCD){
		return;
	}
	setPos(x,y,x+1,y+1);
	WriteCmd(0x2C);
	WriteData(color & 0xFF);
	WriteData(color >> 8);	
}

void drawChar(uint8_t x, uint8_t y, char ch, FontDef font, uint16_t color, uint16_t backGround ){
	uint16_t i,j;
	uint16_t pixelData;
	for(i=0; i<font.height;i++){
		pixelData = font.data[(ch-POSITION_OF_NUMBERS_IN_ASCII)*font.height+i];
		for(j=0; j<font.width;j++){
			if((pixelData<<j) & 0x8000){
				drawPixel(x+j,y+i,color);
			}
			else{
				drawPixel(x+j,y+i,backGround);
			}
		}
	}

}

void drawString(uint8_t x, uint8_t y, char *str, FontDef font, uint16_t color, uint16_t backGround){
	while (*str){
		drawChar(x,y,*str,font,color,backGround);
		x+=font.width;
		str ++;
	}

}


void uint8_to_string(uint8_t number, char *str) {
		if(number == 0){
			str[0]='0';
			str[1]='0';
			str[2]='\0';
			return;
			
		}
    uint8_t temp = number;
    uint8_t cnt = 0;
    while (temp > 0) {
        temp = temp / 10;
        cnt++;
    }
    temp = number;
    for (uint8_t i = 0; i < cnt; i++) {
        uint8_t digit = temp % 10; 
        str[cnt - 1 - i] = digit + 48;        
        temp = temp / 10; 
    }

    str[cnt] = '\0'; 
}
