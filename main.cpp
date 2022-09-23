#include "./display/win32.h"
#include "./draw/line.h"
#include "./draw/polygon.h"
#include <iostream>

Point2I* polygon;

void init() {
    lineSimple(100, 100, 150, 400, 255, 0, 0);
    lineSimple2(200, 100, 250, 400, 255, 0, 0);
//    int size = 6;
//    polygon = new Point2I[size];
//    int i = 0;
//    polygon[i++].setXY(2, 2);
//    polygon[i++].setXY(5, 1);
//    polygon[i++].setXY(11, 3);
//    polygon[i++].setXY(11, 8);
//    polygon[i++].setXY(5, 5);
//    polygon[i  ].setXY(2, 7);

    int size = 4;
    polygon = new Point2I[size];
    int i = 0;
    polygon[i++].setXY(1, 1);
    polygon[i++].setXY(10, 1);
    polygon[i++].setXY(10, 10);
    polygon[i++].setXY(1, 10);

    // transform
    for (i = 0; i < size; ++i) {
        polygon[i].x *= 20;
        polygon[i].x += 300;
        polygon[i].y *= 20;
        polygon[i].y += 200;
    }
    polygonScanLine(polygon, size, 0, 255, 255);
}

void destroy() {
    delete polygon;
}

void onKeyDown(WPARAM key) {}

void onMouseMove(WORD x, WORD y) {}

void onMouseDown(WORD x, WORD y) {}

void onMouseUp(WORD x, WORD y) {}

void onTimer(WPARAM id) {}