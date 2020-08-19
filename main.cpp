#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "rect.h" 
#include <vector>


AnalogIn pot1(A0); // Player 1 input
AnalogIn pot2(A1); // Player 2 input
DigitalIn btn(D4); // Reset button
InterruptIn pBtn(USER_BUTTON); // Pause button
PwmOut buzzer(D8);

float pPos1 = 0.0f;
float pPos2 = 0.0f;
int ballXDir = 1;
int ballYDir = 1;
int score1 = 0;
int score2 = 0;
int startFPS = 60; // Default: 60
int fps; 
int fpsMax = 2000;
int fpsIncrease = 20;
int speed = 1; //Default: 1
int speedMax = 9;
int ballX;
int ballY;
int ballXLast;
int ballYLast;

int frameCount = 0;
bool run = true;
bool showMenu = true;
bool soundToggle = false;

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


void playSound(float wave) {
    if (soundToggle) {
        buzzer = wave;
        ThisThread::sleep_for(20ms);
        buzzer = 0;
    }
}

// Event for toggling pause
void toggleRun() { 
    run = !run;
}

// The visual timer
void timer(){ 
    while (1) {
        if (run) {
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
}

// Resets the timer
void resetTimer() {
    secondC = 0;
    minuteC = 0;
    sprintf(timerStr, "00:00");
}

// Increment speed
void increaseSpeed(int amount) {
    if (fps < fpsMax)
        fps += amount;    
}

// Clear ball visual
void ballClear() {
    BSP_LCD_FillRect(ballXLast, ballYLast, 10, 10);
}

// Each frame
void frame() {
    frameCount++;

    if (btn) {
        p1Score = 0;
        p2Score = 0;
        fps = startFPS;
        ballX = BSP_LCD_GetXSize()/2;
        ballY = BSP_LCD_GetYSize()/2;
        ballXDir = 1;
        ballYDir = 1;
        resetTimer();
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
    
    

    // Player 1 move
    if (pot1>0.8) {
        pPos1=0.8;
    } else if (pot1<0.05) {
        pPos1=0.05;
    } else {
         pPos1=pot1;
    }


    // Player 2 move
    if (pot2>0.8) {
        pPos2=0.8;
    } else if (pot2<0.05f) {
        pPos2=0.05f;
    } else {
         pPos2=pot2;
    }


    p1Y = BSP_LCD_GetYSize()*pPos1;
    p2Y = BSP_LCD_GetYSize()*pPos2;

    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

    // Ball hitbox
    if (ballX >= BSP_LCD_GetXSize()-13) {
        ballXDir = -1;
        p1Score++;
        fps = startFPS;
        printf("p1 score: %i\n", p1Score);
        ballClear();
        ballY = BSP_LCD_GetYSize()/2;
        ballX = BSP_LCD_GetXSize()/2;
        playSound(1.0);
        ThisThread::sleep_for(100ms);
        playSound(1.0);
    } else if (ballX <= 13) {
        ballXDir = 1;
        p2Score++;
        fps = startFPS;
        printf("p2 score: %i\n", p2Score);
        ballClear();
        ballY = BSP_LCD_GetYSize()/2;
        ballX = BSP_LCD_GetXSize()/2;
        playSound(1.0);
        ThisThread::sleep_for(100ms);
        playSound(1.0);
    } else if (ballX >= p1X && ballX <= p1X+10) {
        if (ballY >= p1Y && ballY <= p1Y+p1H) {
            ballXDir = 1;
            playSound(1.0);
        }
    } else if (ballX+10 >= p2X && ballX <= p2X) {
        if (ballY >= p2Y && ballY <= p2Y+p2H) {
            ballXDir = -1;
            playSound(1.0);
        }
    }


    if (ballY >= BSP_LCD_GetYSize()-13) {
        ballYDir = -1;
        playSound(0.2);
    } else if (ballY <= 5) {
        ballYDir = 1;
        playSound(0.2);
    }


    // Translate ball across screen
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
    
    // Save position of ball to clear it next frame.
    ballXLast = ballX;
    ballYLast = ballY;

    HAL_Delay(1000/fps);
}

// Visual menu
void menu() {
    
    rect button1 = rect(50,50,100,50,(uint8_t *)"Start");
    rect button2 = rect(50,100,100,50,(uint8_t *)"Speed");
    rect button3 = rect(50,150,100,50,(uint8_t *)"FPS");
    rect button4 = rect(50,200,100,50,(uint8_t *)"Sound");
    
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    
    BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"Pong", CENTER_MODE);

  bool status = BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
  if (status) {
    printf("[Touch input] Load Successfull!\n");
  } else {
    printf("[Touch input] Load Failed...\n");
    printf("[Touch input] If the touch input works, you can ignore this.\n");
  }

  TS_StateTypeDef TS_State;
  int x = 0;
  int y = 0;
  bool sleepAtEnd = false;
  char txt[10];
  while (showMenu) {

    
    BSP_TS_GetState(&TS_State);
    if (TS_State.touchDetected) {
      for (int idx = 0; idx < TS_State.touchDetected;
           idx++) { 
        x = TS_State.touchX[idx];
        y = TS_State.touchY[idx];
        // printf("Touch %d: x=%d y=%d\n", idx+1, x, y);

        if (button1.isTouched(x, y)) {
            showMenu = false;
        }

        if (button2.isTouched(x, y) && speed < speedMax) {
            speed++;
            sleepAtEnd = true;
        }

        if (button3.isTouched(x, y) && startFPS < fpsMax) {
            startFPS+=fpsIncrease;
            sleepAtEnd = true;
        }

        if (button4.isTouched(x, y)) {
            soundToggle ? soundToggle = false : soundToggle = true;
            sleepAtEnd = true;
        }
        
        }
    }

    sprintf(txt,"Current: %i", speed);
    BSP_LCD_DisplayStringAt(160, 110, (uint8_t *)txt, LEFT_MODE);
    

    sprintf(txt,"Current: %i", startFPS);
    BSP_LCD_DisplayStringAt(160, 160, (uint8_t *)txt, LEFT_MODE);

    soundToggle ? sprintf(txt,"Current: On ") : sprintf(txt,"Current: Off");
    BSP_LCD_DisplayStringAt(160, 210, (uint8_t *)txt, LEFT_MODE);

    if (sleepAtEnd) {
        ThisThread::sleep_for(250ms);
        sleepAtEnd = false;
    }
        
    
  }
}

// Start of program
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

    menu();
    BSP_LCD_Clear(LCD_COLOR_BLACK);

    t.start(&timer);
    pBtn.rise(&toggleRun);

    fps = startFPS;

    while (true) {
        if (run)
            frame();
    }

}