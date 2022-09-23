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

// 改进：当abs(k)<=1时遍历x，当abs(k)>1时遍历y
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

// 改进：增量 (没有考虑直线abs(k)<=1)
void lineDDA(int x0, int y0, int x1, int y1, int r, int g, int b) {
    double k = (y1 - y0) * 1.0 / (x1 - x0);
    double y;
    int x, xMax;
    // 找到直线左端点
    if (x0 < x1) {
        x = x0;
        y = y0;
        xMax = x1;
    } else {
        x = x1;
        y = y1;
        xMax = x0;
    }
    for (;x <= xMax; x++) {
        // (int) (y + 0.5) 也可进行四舍五入
        setPixel(x, (int) round(y), r, g, b);
        y += k;
    }
}

// 改进：考虑了abs(k)>1
void lineDDA2(int x0, int y0, int x1, int y1, int r, int g, int b) {
    double incX, incY, x, y;
    int steps = std::abs(x0 - x1) > std::abs(y0 - y1) ? std::abs(x1 - x0) : std::abs(y1 - y0);
    incX = (x1 - x0) * 1.0 / steps;
    incY = (y1 - y0) * 1.0 / steps;
    x = x0;
    y = y0;
    for (int i = 0; i < steps; i++) {
        setPixel((int) x, (int) y, r, g, b);
        x += incX;
        y += incY;
    }
}

// 改进：计算转变为判断 (默认x0<x1, abs(k)<=1)
void lineBresenham(int x0, int y0, int x1, int y1, int r, int g, int b) {
    double k = (y1 - y0) * 1.0 / (x1 - x0);
    double e = -0.5;
    int y = y0;
    for (int x = x0; x <= x1; x++) {
        setPixel(x, y, r, g, b);
        e += k;
        if (e >= 0) {
            y ++;
            e -= 1;
        }
    }
}

// 改进：浮点运算转为整数运算 (默认x0<x1, abs(k)<=1)
void lineBresenham2(int x0, int y0, int x1, int y1, int r, int g, int b) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int e = -dx;
    int x = x0;
    int y = y0;
    for (int i = 0; i < dx; i++) {
        setPixel(x, y, r, g, b);
        x ++;
        e += 2 * dy;
        if (e >= 0) {
            y ++;
            e -= 2 * dx;
        }
    }
}
