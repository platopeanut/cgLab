/**
* @Author: Peanut
* @Date: 2022/9/22 19:21
* @Email: platopeanut@gmail.com
*/

#include <cmath>
#include <iostream>
#include "line.h"
#include "../display/win32.h"

inline void swap(int& a, int& b) {
    int tmp = a;
    a = b;
    b = tmp;
}

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
    if (x0 > x1) {
        swap(x0, x1);
        swap(y0, y1);
    }
    double k = (y1 - y0) * 1.0 / (x1 - x0);
    int x = x0;
    double y = y0;
    for (;x <= x1; x++) {
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

// 改进：计算转变为判断         限制：0 <= k <= 1
void lineBresenham(int x0, int y0, int x1, int y1, int r, int g, int b) {
    if (x0 > x1) {
        swap(x0, x1);
        swap(y0, y1);
    }
    double k = (y1 - y0) * 1.0 / (x1 - x0);
    double e = -0.5;
    int x = x0, y = y0;
    for (; x <= x1; x++) {
        setPixel(x, y, r, g, b);
        e += k;
        if (e >= 0) {
            y ++;
            e -= 1;
        }
    }
}

// 改进：浮点运算转为整数运算
void lineBresenham2(int x0, int y0, int x1, int y1, int r, int g, int b) {
    if (x0 > x1) {
        swap(x0, x1);
        swap(y0, y1);
    }
    bool negativeFlag = false;      // when k < 0
    if (y1 < y0) {
        negativeFlag = true;
        y1 = 2 * y0 - y1;
    }
    bool steepFlag = false;         // when abs(k) > 1
    if (y1 - y0 > x1 - x0) {
        steepFlag = true;
        int tmpX1 = x1;
        x1 = y1 - y0 + x0;
        y1 = tmpX1 - x0 + y0;
    }
    int x = x0, y = y0;
    int dx = x1 - x0;
    int dy = y1 - y0;
    int e = -dx;
    for (int i = 0; i < dx; i++) {
        int finalX = x, finalY = y;
        if (steepFlag) {
            int tmpFinalX = finalX;
            finalX = finalY - y0 + x0;
            finalY = tmpFinalX - x0 + y0;
        }
        if (negativeFlag) {
            finalY = 2 * y0 - finalY;
        }
        setPixel(finalX, finalY, r, g, b);
        x ++;
        e += 2 * dy;
        if (e >= 0) {
            y ++;
            e -= 2 * dx;
        }
    }
}

// 使用判别式：kx+m-y>0?      限制：0 <= k <= 1
void lineMidPoint(int x0, int y0, int x1, int y1, int r, int g, int b) {
    if (x0 > x1) {
        swap(x0, x1);
        swap(y0, y1);
    }
    int x = x0;
    int y = y0;
    double k = (y1 - y0) * 1.0 / (x1 - x0);
    double d = k;
    setPixel(x, y, r, g, b);
    while (x <= x1) {
        if (d > 0) {
            x ++;
            y ++;
            d += k - 1;
        } else {
            x ++;
            d += k;
        }
        setPixel(x, y, r, g, b);
    }
}

// 使用判别式：Ax+By+C>0?     改进：避免浮点运算
void lineMidPoint2(int x0, int y0, int x1, int y1, int r, int g, int b) {
    if (x0 > x1) {
        swap(x0, x1);
        swap(y0, y1);
    }
    bool negativeFlag = false;      // when k < 0
    if (y1 < y0) {
        negativeFlag = true;
        y1 = 2 * y0 - y1;
    }
    bool steepFlag = false;         // when abs(k) > 1
    if (y1 - y0 > x1 - x0) {
        steepFlag = true;
        int tmpX1 = x1;
        x1 = y1 - y0 + x0;
        y1 = tmpX1 - x0 + y0;
    }
    int A = y0 - y1;
    int B = x1 - x0;
    int d = 2 * A + B;
    int d1 = 2 * A;         // to right
    int d2 = 2 * (A + B);   // to up
    int x = x0, y = y0;
    setPixel(x, y, r, g, b);
    while (x <= x1) {
        if (d < 0) {
            x ++;
            y ++;
            d += d2;
        } else {
            x ++;
            d += d1;
        }
        int finalX = x, finalY = y;
        if (steepFlag) {
            int tmpFinalX = finalX;
            finalX = finalY - y0 + x0;
            finalY = tmpFinalX - x0 + y0;
        }
        if (negativeFlag) {
            finalY = 2 * y0 - finalY;
        }
        setPixel(finalX, finalY, r, g, b);
    }
}

// 基于lineSimple的反走样算法，width为线宽，默认为1，限制 0 <= k <= 1
void lineSimpleAntiAliasing(int x0, int y0, int x1, int y1, int r, int g, int b, int width, int mode, int samples) {
    if (x0 > x1) {
        swap(x0, x1);
        swap(y0, y1);
    }
    double k = (y1 - y0) * 1.0 / (x1 - x0);
    double m = y0 - k * x0;
    double half = width * 0.5 * std::sqrt(k * k + 1);
    double mUp = m + half;
    double mDown = m - half;
    for (int x = x0; x <= x1; x++) {
        double yUp = k * x + mUp;
        double yDown = k * x + mDown;
        for (int y = (int) yDown + 1; y < (int) yUp; ++y) {
            setPixel(x, y, r, g, b);
        }
        double w;
        int alpha = 2;
        for (int i = 0; i < alpha; ++i) {
            if (mode == 0) w = sampleColor(k, mUp, mDown, x, (int) (yDown - i), samples);
            else if (mode == 1) w = sampleColorWeighted(k, mUp, mDown, x, (int) (yDown - i));
            setPixel(x, (int) (yDown - i), (int)(w * r), (int)(w * g), (int)(w * b));
            if (mode == 0) w = sampleColor(k, mUp, mDown, x, (int) (yUp + i), samples);
            else if (mode == 1) w = sampleColorWeighted(k, mUp, mDown, x, (int) (yUp + i));
            setPixel(x, (int) (yUp + i), (int)(w * r), (int)(w * g), (int)(w * b));
        }
    }
}

// samples 采样数
double sampleColor(double k, double mUp, double mDown, int x, int y, int samples) {
    double d = 1.0 / samples;           // 两个采样点之间的距离
    double baseX = x - 0.5 + d * 0.5;   // 左下采样点的x
    double baseY = y - 0.5 + d * 0.5;   // 左下采样点的y
    int cnt = 0;                        // 符合的采样点的个数
    for (int i = 0; i < samples; ++i) {
        for (int j = 0; j < samples; ++j) {
            double currX = baseX + i * d;
            double currY = baseY + j * d;
            double currM = currY - k * currX;
            if (currM <= mUp && currM >= mDown) cnt ++;
        }
    }
    return cnt * 1.0 / (samples * samples);
}

double sampleColorWeighted(double k, double mUp, double mDown, int x, int y) {
    int samples = 3;                    // 固定采样数为3 * 3
    double d = 1.0 / samples;           // 两个采样点之间的距离
    double baseX = x - 0.5 + d * 0.5;   // 左下采样点的x
    double baseY = y - 0.5 + d * 0.5;   // 左下采样点的y
    int value = 0;
    int weightMat[3][3] = {
            {1, 2, 1},
            {2, 4, 2},
            {1, 2, 1}
    };
    for (int i = 0; i < samples; ++i) {
        for (int j = 0; j < samples; ++j) {
            double currX = baseX + i * d;
            double currY = baseY + j * d;
            double currM = currY - k * currX;
            if (currM <= mUp && currM >= mDown)
                value += weightMat[i][j];
        }
    }
    return value * 1.0 / 16;
}
