#include "mbed.h"
#include "stm32746g_discovery_lcd.h"

AnalogIn pot1(A0);
AnalogIn pot2(A1);
float pPos1 = 0.0f;
float pPos2 = 0.0f;
int p2goingUp = false;
int ballXDir = 1;
int ballYDir = 1;
int score1 = 0;
int score2 = 0;
int fps = 60;
int ballX;
int ballY;


int p1X;
int p1Y;
int p1H = 50;

int p2X;
int p2Y;
int p2H = 50;

int p1Score = 0;
int p2Score = 0;

void ballClear() {
    BSP_LCD_FillRect(ballX-2, ballY-2, 14, 14);
}

void frame() {

    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    // Player 1 clear
    BSP_LCD_FillRect(p1X, 0, 10, BSP_LCD_GetYSize());
    // Player 2 clear
    BSP_LCD_FillRect(p2X, 0, 10, BSP_LCD_GetYSize());


    //char* p1ScoreArr[8];
    //printf((char *)p1ScoreArr, "Score: %d", p1Score);

    //char* p2ScoreArr[8];
    //printf((char *)p2ScoreArr, "Score: %d", p2Score);


    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/10, (uint8_t *)"Ping Pong",CENTER_MODE);
    //BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/10, (uint8_t *)p1ScoreArr,LEFT_MODE);
    //BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/10, (uint8_t *)p2ScoreArr,RIGHT_MODE);
    
    


    if (pot1>0.8f) {
        pPos1=0.8f;
    } else if (pot1<0.05f) {
        pPos1=0.05f;
    } else {
         pPos1=pot1;
    }


    // For pvp
    if (pot2>0.8f) {
        pPos2=0.8f;
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
        printf("p1 score increased\n");
        ballClear();
        ballY = BSP_LCD_GetYSize()/2;
        ballX = BSP_LCD_GetXSize()/2;
    } else if (ballX <= 2) {
        ballXDir = 1;
        p2Score++;
        printf("p2 score increased\n");
        ballClear();
        ballY = BSP_LCD_GetYSize()/2;
        ballX = BSP_LCD_GetXSize()/2;
    } else if (ballX-10 == p1X) {
        if (ballY >= p1Y && ballY <= p1Y+p1H) {
            ballXDir = 1;
        }
    } else if (ballX == p2X-10) {
        if (ballY >= p2Y && ballY <= p2Y+p2H) {
            ballXDir = -1;
        }
    }


    if (ballY >= BSP_LCD_GetYSize()-13) {
        ballYDir = -1;
    } else if (ballY <= 2) {
        ballYDir = 1;
    }

    ballX += ballXDir*2;
    ballY += ballYDir*2;

    
    
    // Ball clear
    ballClear();

    


    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    // Player 1 draw
    BSP_LCD_FillRect(p1X, p1Y, 10, p1H);
    // Player 2 draw
    BSP_LCD_FillRect(p2X, p2Y, 10, p2H);
    // Ball draw
    BSP_LCD_FillRect(ballX, ballY, 10, 10);

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
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    

    ballX = BSP_LCD_GetXSize()/2;
    ballY = BSP_LCD_GetYSize()/2;

    p1X = BSP_LCD_GetXSize()/10;
    p2X = BSP_LCD_GetXSize()/10*9;
    
    while (true) {
        frame();
    }
}

