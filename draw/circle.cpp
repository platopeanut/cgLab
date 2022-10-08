/**
* @Author: Peanut
* @Date: 2022/9/23 18:00
* @Email: platopeanut@gmail.com
*/

#include "circle.h"
#include "../display/win32.h"
#include <cmath>
#include <iostream>


void circleSimple(int x, int y, int radius, int r, int g, int b) {
    for (int i = x - radius; i <= x + radius; i++) {
        double tmp = std::sqrt(radius * radius - (i - x) * (i - x));
        setPixel(i, (int)std::round(y+tmp), r, g, b);
        setPixel(i, (int)std::round(y-tmp), r, g, b);
    }
}

void circleSimple2(int x, int y, int radius, int r, int g, int b) {
    int curr_x = x;
    int curr_y = y + radius;
    while (curr_x - x <= curr_y - y) {
        setPixel(curr_x, curr_y, r, g, b);
        eightWaySymmetry(x, y, curr_x, curr_y, r, g, b);
        curr_x ++;
        curr_y = (int) std::round(y + std::sqrt(radius * radius - (curr_x - x) * (curr_x - x)));
    }
}

void circleMidPoint(int x, int y, int radius, int r, int g, int b) {
    int curr_x = x;
    int curr_y = y + radius;
    while (curr_x - x <= curr_y - y) {
        setPixel(curr_x, curr_y, r, g, b);
        eightWaySymmetry(x, y, curr_x, curr_y, r, g, b);
        curr_x ++;
        double d = (curr_x - x) * (curr_x - x) + (curr_y - 0.5 - y) * (curr_y - 0.5 - y) - radius * radius;
        if (d >= 0) curr_y --;
    }
}

void circleMidPoint2(int x, int y, int radius, int r, int g, int b) {
    int curr_x = 0;
    int curr_y = radius;
    double d = 1.25 - radius;
    while (curr_x <= curr_y) {
        setPixel(curr_x+x, curr_y+y, r, g, b);
        eightWaySymmetry(x, y, curr_x+x, curr_y+y, r, g, b);
        if (d < 0) d += 2 * curr_x + 3;
        else {
            d += 2 * (curr_x - curr_y) + 5;
            curr_y --;
        }
        curr_x ++;
    }
}

// 1 / 4 对称 (cx, cy) 为圆心坐标 (x, y) 为圆弧上一点 该函数画出其余三个点
void fourWaySymmetry(int cx, int cy, int x, int y, int r, int g, int b) {
    setPixel(cx+cx-x, cy+cy-y, r, g, b);
    setPixel(x, cy+cy-y, r, g, b);
    setPixel(cx+cx-x, y, r, g, b);
}

// 1 / 8 对称 (cx, cy) 为圆心坐标 (x, y) 为圆弧上一点 该函数画出其余七个点
void eightWaySymmetry(int cx, int cy, int x, int y, int r, int g, int b) {
    setPixel(cx+cx-x, cy+cy-y, r, g, b);
    setPixel(x, cy+cy-y, r, g, b);
    setPixel(cx+cx-x, y, r, g, b);
    setPixel(cx-cy+y, cy+cx-x, r, g, b);
    setPixel(cx+cy-y, cy-cx+x, r, g, b);
    setPixel(cx-cy+y, cy-cx+x, r, g, b);
    setPixel(cx+cy-y, cy+cx-x, r, g, b);
}

void circleBresenham(int x, int y, int radius, int r, int g, int b) {
    int currX = 0;
    int currY = radius;
    double deltaD = 2 * (1 - radius);
    // 1 / 4 circle
    while (currY >= 0) {
        setPixel(x + currX, y + currY, r, g, b);
        fourWaySymmetry(x, y, x + currX, y + currY, r, g, b);
        int mode;
        if (deltaD < 0) {
            mode = 2 * (deltaD + currY) - 1 <= 0 ? 0 : 2;
        } else if (deltaD > 0) {
            mode = 2 * (deltaD - currX) - 1 >= 0 ? 1 : 2;
        } else mode = 2;
        if (mode == 0) {
            // H
            currX ++;
            deltaD += 2 * currX + 1;
        }
        else if (mode == 1) {
            // V
            currY --;
            deltaD += -2 * currY + 1;
        }
        else {
            // D
            currX ++;
            currY --;
            deltaD += 2 * (currX - currY + 1);
        }
    }
}

void ellipseMidPoint(int x, int y, int A, int B, int r, int g, int b) {
    int currX = 0;
    int currY = B;
    double d = B * B + A * A * (-B + 0.25);
    // 上半
    while (B * B * (currX + 1) < A * A * (currY - 0.5)) {
        setPixel(x + currX, y + currY, r, g, b);
        fourWaySymmetry(x, y, x + currX, y + currY, r, g, b);
        if (d <= 0) {
            d += B * B * (2 * currX + 3);
        } else {
            d += B * B * (2 * currX + 3) + A * A * (-2 * currY + 2);
            currY --;
        }
        currX ++;
    }
    // 下半
    d = B * B * currX * currX + A * A * (currY - 0.5) * (currY - 0.5) - A * A * B * B;
    while (currY >= 0) {
        setPixel(x + currX, y + currY, r, g, b);
        fourWaySymmetry(x, y, x + currX, y + currY, r, g, b);
        if (d > 0) {
            d += A * A * (-2 * currY + 3);
        } else {
            d += B * B * (2 * currX + 2) + A * A * (-2 * currY + 3);
            currX ++;
        }
        currY --;
    }
}

