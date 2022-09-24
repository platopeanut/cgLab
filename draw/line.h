/**
* @Author: Peanut
* @Date: 2022/9/22 19:21
* @Email: platopeanut@gmail.com
*/

#ifndef CGLAB_LINE_H
#define CGLAB_LINE_H

// 定义画线函数的函数指针类型
typedef void (*fpDrawLine)(int x0, int y0, int x1, int y1, int r, int g, int b);

void lineSimple(int x0, int y0, int x1, int y1, int r, int g, int b);
void lineSimple2(int x0, int y0, int x1, int y1, int r, int g, int b);
void lineDDA(int x0, int y0, int x1, int y1, int r, int g, int b);
void lineDDA2(int x0, int y0, int x1, int y1, int r, int g, int b);
void lineBresenham(int x0, int y0, int x1, int y1, int r, int g, int b);
void lineBresenham2(int x0, int y0, int x1, int y1, int r, int g, int b);

void lineMidPoint(int x0, int y0, int x1, int y1, int r, int g, int b);
void lineMidPoint2(int x0, int y0, int x1, int y1, int r, int g, int b);


#endif //CGLAB_LINE_H
