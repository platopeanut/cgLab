/**
* @Author: Peanut
* @Date: 2022/9/22 19:21
* @Email: platopeanut@gmail.com
*/

#ifndef CGLAB_LINECLIPPING_H
#define CGLAB_LINE_H

// 定义画线函数的函数指针类型
typedef void (*fpDrawLine)(int x0, int y0, int x1, int y1, int r, int g, int b);

void lineSimple(int x0, int y0, int x1, int y1, int r, int g, int b);
void lineSimple2(int x0, int y0, int x1, int y1, int r, int g, int b);
void lineDDA(int x0, int y0, int x1, int y1, int r, int g, int b);
void lineDDA2(int x0, int y0, int x1, int y1, int r, int g, int b);
// 画虚线
void dashLineDDA2(int x0, int y0, int x1, int y1, int r, int g, int b);
void lineBresenham(int x0, int y0, int x1, int y1, int r, int g, int b);
void lineBresenham2(int x0, int y0, int x1, int y1, int r, int g, int b);

void lineMidPoint(int x0, int y0, int x1, int y1, int r, int g, int b);
void lineMidPoint2(int x0, int y0, int x1, int y1, int r, int g, int b);

// 非加权采样
double sampleColor(double k, double mUp, double mDown, int x, int y, int samples);
// 加权采样 固定为3*3
double sampleColorWeighted(double k, double mUp, double mDown, int x, int y);
// mode=0 非加权采样
// mode=1 加权采样
void lineSimpleAntiAliasing(int x0, int y0, int x1, int y1, int r, int g, int b, int width=1, int mode = 0, int samples=2);


#endif //CGLAB_LINECLIPPING_H
