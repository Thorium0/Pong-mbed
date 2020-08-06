#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include <vector>


AnalogIn pot1(A0);
AnalogIn pot2(A1);

InterruptIn btn(D4);

float pPos1 = 0.0f;
float pPos2 = 0.0f;
int p2goingUp = false;
int ballXDir = 1;
int ballYDir = 1;
int score1 = 0;
int score2 = 0;
int fps = 120; // Default: 60
int speed = 1; //Default: 1
int ballX;
int ballY;
int ballXLast;
int ballYLast;

int frameCount = 0;


int p1X;
int p1Y;
int p1H = 50;

int p2X;
int p2Y;
int p2H = 50;

int p1Score = 0;
int p2Score = 0;

Thread t;
int secondC;
int minuteC;

char timerStr[5];

void timer(){
    while (1) {
    char str[5]; 
    if (secondC < 10 && minuteC < 10)
        sprintf(timerStr, "0%i:0%i", minuteC, secondC);
    else if (secondC < 10 && minuteC >= 10)
        sprintf(timerStr, "%i:0%i", minuteC, secondC);
    else if (secondC >= 10 && minuteC < 10)
        sprintf(timerStr, "0%i:%i", minuteC, secondC);
    else
        sprintf(timerStr, "%i:%i", minuteC, secondC);
    
    if (minuteC < 99)
        secondC++;
    if (secondC >= 60 ) {
        secondC = 0;
        minuteC++;
    }
    ThisThread::sleep_for(1s);
    }
}

void increaseSpeed(int amount) {
    if (fps < 480)
        fps += amount;    
}

void ballClear() {
    BSP_LCD_FillRect(ballXLast, ballYLast, 10, 10);
}

void frame() {
    frameCount++;

    if (btn) {
        p1Score = 0;
        p2Score = 0;
        fps = 60;
        BSP_LCD_Clear(LCD_COLOR_BLACK);
        printf("Score reset\n");
    }


    if (frameCount >= 500){
        increaseSpeed(20);
        printf("Speed increased!\n");
        frameCount = 0;
    }
    
    

    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    // Player 1 clear
    BSP_LCD_FillRect(p1X, 0, 10, p1Y-1);
    BSP_LCD_FillRect(p1X, p1Y+p1H+1, 10, BSP_LCD_GetYSize()-p1Y+p1H);
    
    // Player 2 clear
    BSP_LCD_FillRect(p2X, 0, 10, p2Y-1);
    BSP_LCD_FillRect(p2X, p2Y+p2H+1, 10, BSP_LCD_GetYSize()-p2Y+p2H);

    

    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()-20, (uint8_t *)timerStr, CENTER_MODE);
    char str[10];    
    sprintf(str, "%i:%i", p1Score, p2Score);
    BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)str, CENTER_MODE);
    
    


    if (pot1>0.8) {
        pPos1=0.8;
    } else if (pot1<0.05) {
        pPos1=0.05;
    } else {
         pPos1=pot1;
    }


    // For pvp
    if (pot2>0.8) {
        pPos2=0.8;
    } else if (pot2<0.05f) {
        pPos2=0.05f;
    } else {
         pPos2=pot2;
    }


    // For pve
    /* 
    if (pPos2 <= 0.05f) {
        p2goingUp = false;
    } else if (pPos2 >= 0.8f) {
        p2goingUp = true;
    }

    p2goingUp ? pPos2-=0.01f : pPos2+=0.01f; 
    */


    p1Y = BSP_LCD_GetYSize()*pPos1;
    p2Y = BSP_LCD_GetYSize()*pPos2;

    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

    if (ballX >= BSP_LCD_GetXSize()-13) {
        ballXDir = -1;
        p1Score++;
        fps = 60;
        printf("p1 score: %i\n", p1Score);
        ballClear();
        ballY = BSP_LCD_GetYSize()/2;
        ballX = BSP_LCD_GetXSize()/2;
    } else if (ballX <= 2) {
        ballXDir = 1;
        p2Score++;
        fps = 60;
        printf("p2 score: %i\n", p2Score);
        ballClear();
        ballY = BSP_LCD_GetYSize()/2;
        ballX = BSP_LCD_GetXSize()/2;
    } else if (ballX >= p1X && ballX <= p1X+10) {
        if (ballY >= p1Y && ballY <= p1Y+p1H) {
            ballXDir = 1;
        }
    } else if (ballX+10 >= p2X && ballX <= p2X) {
        if (ballY >= p2Y && ballY <= p2Y+p2H) {
            ballXDir = -1;
        }
    }


    if (ballY >= BSP_LCD_GetYSize()-13) {
        ballYDir = -1;
    } else if (ballY <= 5) {
        ballYDir = 1;
    }

    ballX += ballXDir*speed;
    ballY += ballYDir*speed;

    
    
    // Ball clear
    ballClear();

    


    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    // Player 1 draw
    BSP_LCD_FillRect(p1X, p1Y, 10, p1H);
    // Player 2 draw
    BSP_LCD_FillRect(p2X, p2Y, 10, p2H);
    // Ball draw
    BSP_LCD_FillRect(ballX, ballY, 10, 10);
    
    ballXLast = ballX;
    ballYLast = ballY;

    HAL_Delay(1000/fps);
}




int main()
{
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);  // Sets background of text
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    

    ballX = BSP_LCD_GetXSize()/2;
    ballY = BSP_LCD_GetYSize()/2;

    p1X = BSP_LCD_GetXSize()/10;
    p2X = BSP_LCD_GetXSize()/10*9;

    t.start(&timer);
    
    while (true) {
        frame();
    }
}

