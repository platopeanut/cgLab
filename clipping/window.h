/**
* @Author: Peanut
* @Date: 2022/10/10 22:29
* @Email: platopeanut@gmail.com
*/

#ifndef CGLAB_WINDOW_H
#define CGLAB_WINDOW_H

#include "../display/win32.h"

// unique four bit region code
typedef unsigned char rgCode_t;
// particular window
typedef struct {
    int xMin;
    int xMax;
    int yMin;
    int yMax;
} BBox_t;
// line structure
// 定义x1为-1时该line无效
typedef struct {
    int x1;
    int y1;
    int x2;
    int y2;
} Line_t;
// polygon structure
typedef struct {
    int* x;             // list of points.x
    int* y;             // list of points.y
    int size;           // num of points (polygon is invalid if size == -1)
} Polygon_t;

void drawBBox(const BBox_t* bbox, int r, int g, int b);

bool pointClipping(const BBox_t* bbox, int x, int y);


#endif //CGLAB_WINDOW_H
