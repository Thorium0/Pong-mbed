#include "mbed.h"
#include "stm32746g_discovery_lcd.h"

AnalogIn pot1(A0);
float pPos1 = 0.0f;
float pPos2 = 0.0f;
int goingUp = false;
int score1 = 0;
int score2 = 0;
int fps = 60;


void frame() {
    //BSP_LCD_Clear(LCD_COLOR_BLACK);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/10, (uint8_t *)"Ping Pong",CENTER_MODE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_FillRect(BSP_LCD_GetXSize()/10*9, 0, 10, BSP_LCD_GetYSize());
    BSP_LCD_FillRect(BSP_LCD_GetXSize()/10, 0, 10, BSP_LCD_GetYSize());
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

    if (pot1>0.8f) {
        pPos1=0.8f;
    } else if (pot1<0.05f) {
        pPos1=0.05f;
    } else {
         pPos1=pot1;
    }


    if (pPos2 <= 0.05f) {
        goingUp = false;
    } else if (pPos2 >=0.8f) {
        goingUp = true;
    }

    goingUp ? pPos2-=0.01f : pPos2+=0.01f; 



    BSP_LCD_FillRect(BSP_LCD_GetXSize()/10, BSP_LCD_GetYSize()*pPos1, 10, 50);
    BSP_LCD_FillRect(BSP_LCD_GetXSize()/10*9, BSP_LCD_GetYSize()*pPos2, 10, 50);
    HAL_Delay(1000/fps);
    //printf("pot: %f", pot1);
}

int main()
{
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);  // Sets background of text
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    
    while (true) {
        frame();
    }
}

