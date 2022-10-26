/**
* @Author: Peanut
* @Date: 2022/9/23 17:37
* @Email: platopeanut@gmail.com
*/

#include "./display/win32.h"
#include "./draw/polygon.h"
#include "./draw/line.h"
#include <cstdlib>
#include <vector>

std::vector<Point2I> points;
Point2I* convertToArray() {
    auto* p = (Point2I*) malloc(sizeof(Point2I) * points.size());
    for (int i = 0; i < points.size(); ++i) {
        p[i].x = points[i].x;
        p[i].y = points[i].y;
    }
    return p;
}
Bucket* AET = nullptr;
int mode = 0;   // 0画polygon    1种子填充
int lastX = -1, lastY = -1;

// 绘制color=(r,g,b)的多变形边框
void drawBorder(int r, int g, int b) {
    auto size = points.size();
    if (size > 1) {
        for (int i = 0; i < points.size() - 1; ++i) {
            lineDDA2(points[i].x, points[i].y, points[i+1].x, points[i+1].y, r, g, b);
        }
    }
    if (size > 2) {
        dashLineDDA2(points[size - 1].x,
                     points[size - 1].y,
                     points[0].x,
                     points[0].y,
                     r, g, b);
    }
}

void init() {
//    int x[] = {2,9,12,12,11,10,8,5,5,4,2,1,1};
//    int y[] = {10,10,7,2,1,1,3,3,2,1,1,2,9};
//    int size = sizeof(x) / sizeof(int);
//    auto* ps = (Point2I*) malloc(sizeof(Point2I) * size);
//    for (int i = 0; i < size; ++i) {
//        ps[i].x = x[i];
//        ps[i].y = y[i];
//    }
//    Bucket* aet = polygonScanLine(ps, size, 255, 0, 0);
//    polygonSeedFilling(aet, 7, 8, 0, 255, 0);
}

void destroy() {

}

void onMouseDown(WORD x, WORD y) {
    y = HEIGHT - y;
    if (mode == 0) {
        if (points.empty()) {
            setPixel(x, y, 255, 255, 255);
        }
        if (points.size() == 1) {
            setPixel(points[0].x, points[0].y, 0, 0, 0);
        }

        drawBorder(0, 0, 0);
        points.emplace_back(x, y);
        drawBorder(255, 255, 255);
        update();
    }
    else if (mode == 1 && AET != nullptr) {
        // way1: scan-line filling
        shaderAET(AET, 0, 255, 0);

        // way2: seed filling
//        polygonSeedFilling(AET, x, y, 0, 255, 0);
        Bucket::freeBuckets(AET);
        AET = nullptr;
        update();
    }
}

void onMouseMove(WORD x, WORD y) {
    y = HEIGHT - y;
    if (!points.empty()) {
        Point2I lastP = points.back();
        if (lastX != -1 && lastY != -1) {
            dashLineDDA2(lastP.x, lastP.y, lastX, lastY, 0, 0, 0);
            dashLineDDA2(lastX, lastY, points[0].x, points[0].y, 0, 0, 0);
            drawBorder(255, 255, 255);
        }
        dashLineDDA2(lastP.x, lastP.y, x, y, 208, 64, 30);
        if (points.size() > 2)
            dashLineDDA2(x, y, points[0].x, points[0].y, 208, 64, 30);
        lastX = x;
        lastY = y;
        update();
    }
}

void onMouseUp(WORD x, WORD y) {}

void onKeyDown(WPARAM key) {
    if (key == '0') mode = 0;
    if (key == '1') mode = 1;
    if (key == VK_RETURN) {
        if (mode == 0) {
            drawBorder(0, 0, 0);
            Point2I lastP = points.back();
            dashLineDDA2(lastP.x, lastP.y, lastX, lastY, 0, 0, 0);
            dashLineDDA2(lastX, lastY, points[0].x, points[0].y, 0, 0, 0);
            Point2I* p = convertToArray();
            AET = polygonScanLine(p, (int) points.size(), 255, 0, 0);
            update();
            points.clear();
            free(p);
        }
    }
}