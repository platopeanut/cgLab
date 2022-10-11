/**
* @Author: Peanut
* @Date: 2022/10/11 0:13
* @Email: platopeanut@gmail.com
*/

#ifndef CGLAB_LINECLIPPING_H
#define CGLAB_LINECLIPPING_H

#include "./window.h"

// 根据bbox计算点(x, y)的区域码
rgCode_t regionEncode(const BBox_t* bbox, int x, int y);

void lineClippingCohenSutherland(const BBox_t* bbox, const Line_t* srcLine, Line_t* dstLine);

#endif //CGLAB_LINECLIPPING_H
