/**
* @Author: Peanut
* @Date: 2022/10/8 22:06
* @Email: platopeanut@gmail.com
*/

#include "others.h"
#include "../display/win32.h"

void squareSimple(int x1, int y1, int x2, int y2, int r, int g, int b) {
    for (int x = x1; x <= x2; x++) {
        setPixel(x, y1, r, g, b);
        setPixel(x, y2, r, g, b);
    }
    for (int y = y2; y <= y1; y++) {
        setPixel(x1, y, r, g, b);
        setPixel(x2, y, r, g, b);
    }
}
