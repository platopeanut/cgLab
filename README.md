# Computer Graphics Lab in CQU

## Branch Info

chp3: Two-dimensional Clipping

## Implemented Algorithms

- Line_t
  - [x] Cohen-Sutherland
  - [ ] Mid-Point
  - [ ] Liang-Barsky
- Polygon
  - [ ] Sutlerland_Hodgman
  - [ ] Weiler-Athenton

## Simple Polygon Clipping Test

~~~cpp
BBox_t bbox = {0, 4, 0, 2};
int x[] = {-1, 2, 2, 5, 3};
int y[] = {0, 3, -1, 1, -4};
Polygon_t polygon = {x, y, 5};
int size;
Polygon_t* polygons = polygonClippingWeilerAthenton(&bbox, &polygon, &size);
for (int i = 0; i < size; ++i) {
    std::cout << "\npolygon:" << std::endl;
    for (int j = 0; j < polygons[i].size; ++j) {
        std::cout << "(" << polygons[i].x[j] << ", " << polygons[i].y[j] << ") ";
    }
}
freePolygons(polygons, size);
~~~

## BackUP
~~~cpp
/**
* @Author: Peanut
* @Date: 2022/9/23 17:37
* @Email: platopeanut@gmail.com
*/

#include "./display/win32.h"
#include "./draw/line.h"
#include "./clipping/window.h"
#include "./clipping/lineClipping.h"
#include <iostream>

// TODO: 添加事件：右键"down-move-up"可以移动window

BBox_t window = {150, 450, 200, 400};
Line_t line = {-1, -1, -1, -1};

void init() {
    drawBBox(&window, 255, 255, 255);
}

void destroy() {}

void onMouseDown(WORD x, WORD y) {
    y = HEIGHT - y;
    std::cout << pointClipping(&window, x, y) << " |" << x << "," << y << std::endl;
    if (line.data[0] == -1) {
        line.data[0] = x;
        line.data[1] = y;
    } else {
        line.data[2] = x;
        line.data[3] = y;
        Line_t cutLine;
        lineClippingCohenSutherland(&window, &line, &cutLine);
        if (cutLine.data[0] != -1) {
            lineDDA2(line.data[0], line.data[1], cutLine.data[0], cutLine.data[1], 255, 0, 0);
            lineDDA2(cutLine.data[0], cutLine.data[1], cutLine.data[2], cutLine.data[3], 0, 255, 0);
            lineDDA2(cutLine.data[2], cutLine.data[3], line.data[2], line.data[3], 255, 0, 0);
        } else {
            lineDDA2(line.data[0], line.data[1], line.data[2], line.data[3], 255, 0, 0);
        }
        line.data[0] = -1;
        update();
    }
}

void onMouseMove(WORD x, WORD y) {}

void onMouseUp(WORD x, WORD y) {}

void onKeyDown(WPARAM key) {}
~~~