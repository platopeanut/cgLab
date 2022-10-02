/**
* @Author: Peanut
* @Date: 2022/9/23 17:37
* @Email: platopeanut@gmail.com
*/

#include "./draw/polygon.h"
#include "./draw/circle.h"
#include "./draw/line.h"
#include "./display/win32.h"
#include <vector>
#include <chrono>

std::vector<Point2I>* polygon;

void init() {
    polygon = new std::vector<Point2I>;
    int width = 10;
    // 加权
    lineSimpleAntiAliasing(100, 150, 400, 250, 255, 255, 255, width, 1);
    // 非加权
    lineSimpleAntiAliasing(100, 200, 400, 300, 255, 255, 255, width, 0, 3);
    for (int i = 0; i < width; ++i)
        lineSimple2(100, 250 + i, 400, 350 + i, 255, 255, 255);
}

void destroy() { delete polygon; }

void onMouseDown(WORD x, WORD y) {
    circleMidPoint2(x, HEIGHT - y, 2, 255, 255, 255);
    update();
    polygon->push_back(Point2I(x, HEIGHT - y));
}

void onKeyDown(WPARAM key) {
    if (key == VK_RETURN) {
        size_t size = polygon->size();
        auto* points = new Point2I[size];
        for (int i = 0; i < size; ++i) {
            points[i] = polygon->back();
            polygon->pop_back();
        }
        auto start = std::chrono::high_resolution_clock::now();
        polygonScanLine(points, (int) size, 255, 255, 255);
        auto finish = std::chrono::high_resolution_clock::now();
        auto cost = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
        setWindowTitle("Cost: " + std::to_string((double) cost / 1e+6) + "ms");
        update();
        delete[] points;
    }
}
