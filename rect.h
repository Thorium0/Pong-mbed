#include "mbed.h"

class rect {
    public:
        rect (int _x, int _y, int _width, int _height, uint8_t* _txt);
        bool isTouched(int touchX, int touchY);

    private: // The following variables only need to be used within the class so there is no need to make them public
        int x;
        int y;
        int width;
        int height;
        uint8_t* txt;
};
