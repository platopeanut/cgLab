/**
* @Author: Peanut
* @Date: 2022/9/23 18:00
* @Email: platopeanut@gmail.com
*/

#ifndef CGLAB_CIRCLE_H
#define CGLAB_CIRCLE_H

// 定义画圆函数的函数指针类型
typedef void (*fpDrawCircle) (int x, int y, int radius, int r, int g, int b);

void circleSimple(int x, int y, int radius, int r, int g, int b);
void circleSimple2(int x, int y, int radius, int r, int g, int b);
void circleMidPoint(int x, int y, int radius, int r, int g, int b);
void circleMidPoint2(int x, int y, int radius, int r, int g, int b);

void eightWaySymmetry(int cx, int cy, int x, int y, int r, int g, int b);

#endif //CGLAB_CIRCLE_H
