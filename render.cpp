/**
* @Author: Peanut
* @Date: 2022/9/23 17:37
* @Email: platopeanut@gmail.com
*/

#include "./display/win32.h"
#include "./draw/polygon.h"
#include <cstdlib>
#include <vector>
#include <iostream>

#define min(a, b) (a) > (b) ? (b) : (a)

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
        setPixel(x, y, 255, 0, 0);
        update();
        points.emplace_back(x, y);
    }
    else if (mode == 1 && AET != nullptr) {
        setPixel(x, y, 0, 255, 0);
        polygonSeedFilling(AET, x, y, 0, 255, 0);
//        shaderAET(AET, 0, 255, 0);
        Bucket::freeBuckets(AET);
        AET = nullptr;
        update();
    }
}

void onMouseMove(WORD x, WORD y) {}

void onMouseUp(WORD x, WORD y) {}

void onKeyDown(WPARAM key) {
    if (key == '0') mode = 0;
    if (key == '1') mode = 1;
    if (key == VK_RETURN) {
        if (mode == 0) {
            Point2I* p = convertToArray();
            AET = polygonScanLine(p, (int) points.size(), 255, 0, 0);
            update();
            points.clear();
            free(p);
        }
    }
}