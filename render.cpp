/**
* @Author: Peanut
* @Date: 2022/9/23 17:37
* @Email: platopeanut@gmail.com
*/

#include "./display/win32.h"
#include "./draw/polygon.h"
#include "./clipping/window.h"
#include "./clipping/polygonClipping.h"
#include <iostream>
#include <vector>
#include <cmath>

// TODO: 添加事件：右键"down-move-up"可以移动window

BBox_t window = {150, 450, 200, 400};
bool moveFlag = false;
std::vector<struct Point> points;

typedef struct { int x, y, z; } Vec3;
// 计算两个三维向量的叉乘
Vec3 calcCrossDot(Vec3 v1, Vec3 v2) {
    Vec3 res;
    res.x = v1.y * v2.z - v2.y * v1.z;
    res.y = v2.x * v1.z - v1.x * v2.z;
    res.z = v1.x * v2.y - v2.x * v1.y;
    return res;
}

// 规范化旋转方向：将输入的顶点集合规范为顺时针转向
void normalizeRotateDir(std::vector<struct Point>& ps) {
    if (ps.size() < 3) return;
    int i = 0;
    Vec3 v;
    do {
        Vec3 v1 = { ps[i+1].x - ps[i].x, ps[i+1].y - ps[i].y, 0};
        Vec3 v2 = { ps[i+2].x - ps[i+1].x, ps[i+2].y - ps[i+1].y, 0};
        v = calcCrossDot(v1, v2);
        i ++;
    } while (v.z == 0);
    // 逆时针旋转，则将数组反向
    if (v.z > 0) {
        for (int j = 0; j < ps.size() / 2; ++j) {
            Point tmp = ps[j];
            ps[j] = ps[ps.size() - 1 - j];
            ps[ps.size() - 1 - j] = tmp;
        }
    }
}


void draw() {
    normalizeRotateDir(points);
    Polygon_t polygon;
    polygon.size = (int) points.size();
    polygon.x = (int*) malloc(sizeof(int) * polygon.size);
    polygon.y = (int*) malloc(sizeof(int) * polygon.size);
    auto* ps = (Point2I*) malloc(sizeof(Point2I) * polygon.size);
    for (int i = 0; i < polygon.size; ++i) {
        polygon.x[i] = points[i].x;
        polygon.y[i] = points[i].y;
        ps[i].x = points[i].x;
        ps[i].y = points[i].y;
    }
    polygonScanLine(ps, polygon.size, 255, 0, 0);
    free(ps);
    int size;
    auto polygons = polygonClippingWeilerAthenton(&window, &polygon, &size);
    for (int i = 0; i < size; ++i) {
        ps = (Point2I*) malloc(sizeof(Point2I) * polygons[i].size);
        for (int j = 0; j < polygons[i].size; ++j) {
            ps[j].x = polygons[i].x[j];
            ps[j].y = polygons[i].y[j];
        }
        polygonScanLine(ps, polygons[i].size, 0, 255, 0);
        free(ps);
    }
}

void clear() {
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            setPixel(i, j, 0, 0, 0);
        }
    }
    drawBBox(&window, 255, 255, 255);
}

void init() {
    drawBBox(&window, 255, 255, 255);
}

void destroy() {}

void onMouseDown(WORD x, WORD y) {
    y = HEIGHT - y;
    struct Point point = {x, y};
    points.push_back(point);
    setPixel(x, y, 255, 0, 0);
    update();
}

void onMouseMove(WORD x, WORD y) {
    y = HEIGHT - y;
    int deltaX = x - (window.xMin + window.xMax) / 2;
    int deltaY = y - (window.yMin + window.yMax) / 2;
    if (moveFlag) {
        window.xMin += deltaX;
        window.xMax += deltaX;
        window.yMin += deltaY;
        window.yMax += deltaY;
        clear();
        if (points.size() >= 3) draw();
        update();
    }
}

void onMouseUp(WORD x, WORD y) {}

void onRMouseDown(WORD x, WORD y) {
    moveFlag = true;
}

void onRMouseUp(WORD x, WORD y) {
    moveFlag = false;
}

void onKeyDown(WPARAM key) {
    if (key == VK_RETURN) {
        draw();
        update();
    }
    else if (key == 'C') {
        clear();
    }
}
