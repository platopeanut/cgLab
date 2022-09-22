#include "./display/win32.h"
#include "./draw/line.h"
#include "./draw/polygon.h"
#include <iostream>

void init() {
    lineSimple(100, 100, 150, 400, 255, 0, 0);
    lineSimple2(200, 100, 250, 400, 255, 0, 0);
    int size = 6;
    auto* vertices = new Point2I[size];
    int i = 0;
    vertices[i++].setXY(2, 2);
    vertices[i++].setXY(5, 1);
    vertices[i++].setXY(11, 3);
    vertices[i++].setXY(11, 8);
    vertices[i++].setXY(5, 5);
    vertices[i  ].setXY(2, 7);
    // transform
    for (i = 0; i < size; ++i) {
        vertices[i].x *= 20;
        vertices[i].x += 300;
        vertices[i].y *= 20;
        vertices[i].y += 200;
    }
    Bucket* NET = buildNET(vertices, size);
    std::cout << "NET:" << std::endl;
    std::cout << *NET;
    buildAET(NET);
    std::cout << "AET:" << std::endl;
    std::cout << *NET << std::endl;
    shaderAET(NET, 0, 255, 0);
}

void destroy() {}

void onKeyDown(WPARAM key) {}

void onMouseMove(WORD x, WORD y) {}

void onMouseDown(WORD x, WORD y) {}

void onMouseUp(WORD x, WORD y) {}

void onTimer(WPARAM id) {}