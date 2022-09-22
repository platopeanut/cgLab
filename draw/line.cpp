/**
* @Author: Peanut
* @Date: 2022/9/22 19:21
* @Email: platopeanut@gmail.com
*/

#include <cmath>
#include "line.h"
#include "../display/win32.h"

void lineSimple(int x0, int y0, int x1, int y1, int r, int g, int b) {
    double k = (y1 - y0) * 1.0 / (x1 - x0);
    double m = y0 - k * x0;
    int maxX = std::max(x0, x1);
    for (int x = std::min(x0, x1); x <= maxX; x++) {
        setPixel(x, (int) round(k * x + m), r, g, b);
    }
}

void lineSimple2(int x0, int y0, int x1, int y1, int r, int g, int b) {
    double k = (y1 - y0) * 1.0 / (x1 - x0);
    if (std::abs(k) <= 1) lineSimple(x0, y0, x1, y1, r, g, b);
    else {
        k = 1 / k;
        double m = x0 - k * y0;
        int maxY = std::max(y0, y1);
        for (int y = std::min(y0, y1); y <= maxY; y++) {
            setPixel((int) round(y * k + m), y, r, g, b);
        }
    }
}

void lineDDA(int x0, int y0, int x1, int y1, int r, int g, int b) {

}

void lineDDA2(int x0, int y0, int x1, int y1, int r, int g, int b) {

}

void lineBresenham(int x0, int y0, int x1, int y1, int r, int g, int b) {

}

void lineBresenham2(int x0, int y0, int x1, int y1, int r, int g, int b) {

}
