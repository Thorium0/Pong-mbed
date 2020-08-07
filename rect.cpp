#include "mbed.h"
#include "rect.h"
#include "stm32746g_discovery_lcd.h"
#include <string>

// Constructor
rect::rect (int _x, int _y, int _width, int _height, uint8_t* _txt) {
  x = _x;
  y = _y;
  width = _width;
  height = _height;
  txt = _txt;
  BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
  BSP_LCD_FillRect(_x-3, _y-3, _width+6, _height+6);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillRect(_x, _y, _width, _height); // Paint Rect
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(_x+height/5, _y+_height/4, _txt, LEFT_MODE);
};
// Checks if touch coordinates are within key object
bool rect::isTouched(int touchX, int touchY) {
  return touchX > x && touchX < x+width && touchY > y && touchY < y+height;
};