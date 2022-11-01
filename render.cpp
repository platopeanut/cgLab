/**
* @Author: Peanut
* @Date: 2022/9/23 17:37
* @Email: platopeanut@gmail.com
*/

#include "./display/win32.h"
#include "./draw/line.h"
#include "./draw/polygon.h"
#include "./clipping/window.h"
#include "./clipping/polygonClipping.h"
#include "./clipping/lineClipping.h"
#include <vector>
#include <cmath>

typedef struct { int x, y, z; } Vec3;

////////////////////////////////////////////////////////////////
BBox_t window = {150, 450, 200, 400};
// 绘制模式：1为线 2为多边形
#define MODE_LINE 1
#define MODE_POLY 2
int mode = MODE_LINE;


bool wndFlag, dirtyFlag;      // window移动还是primitive移动
int lastX, lastY;           // 用于记录鼠标上次移动位置
std::vector<std::vector<struct Point>> scene;
////////////////////////////////////////////////////////////////

void reset() {
    wndFlag = false;
    dirtyFlag = false;
    scene.clear();
    lastX = -1;
    lastY = -1;
}


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

// 绘制多边形边框
void drawBorder(int r, int g, int b) {
    if (mode == MODE_POLY && !scene.empty()) {
        auto points = scene.back();
        auto size = points.size();
        if (size > 1) {
            for (int i = 0; i < points.size() - 1; ++i) {
                lineDDA2(points[i].x, points[i].y, points[i+1].x, points[i+1].y, r, g, b);
            }
        }
        if (size > 2) {
            dashLineDDA2(points[0].x,
                         points[0].y,
                         points[size-1].x,
                         points[size-1].y,
                         r, g, b);
        }
    }
}

// 绘制移动辅助线
void drawGuideLine(int r, int g, int b) {
    if (scene.empty()) return;
    auto points = scene.back();
    if (mode == MODE_POLY) {
        if (!points.empty() && lastX != -1 && lastY != -1) {
            Point lastP = points.back();
            dashLineDDA2(lastP.x, lastP.y, lastX, lastY, r, g, b);
        }
        if (points.size() > 1 && lastX != -1 && lastY != -1) {
            dashLineDDA2(points[0].x,
                         points[0].y,
                         lastX,
                         lastY,
                         r, g, b);
        }
    }
    if (mode == MODE_LINE) {
        if (points.size() == 1) {
            dashLineDDA2(points[0].x, points[0].y, lastX, lastY, r, g, b);
        }
    }
}


void drawScene() {
    auto cnt = scene.size();
    if (dirtyFlag) cnt --;
    for (int k = 0; k < cnt; ++k) {
        auto& points = scene[k];
        // drawScene line
        if (points.size() == 2) {
            Line_t line = {points[0].x, points[0].y, points[1].x, points[1].y};
            Line_t cutLine;
            lineClippingCohenSutherland(&window, &line, &cutLine);
            if (cutLine.data[0] != -1) {
                lineDDA2(line.data[0], line.data[1], cutLine.data[0], cutLine.data[1], 255, 0, 0);
                lineDDA2(cutLine.data[0], cutLine.data[1], cutLine.data[2], cutLine.data[3], 0, 255, 0);
                lineDDA2(cutLine.data[2], cutLine.data[3], line.data[2], line.data[3], 255, 0, 0);
            } else {
                lineDDA2(line.data[0], line.data[1], line.data[2], line.data[3], 255, 0, 0);
            }
        }
            // drawScene polygon
        else if (points.size() > 2) {
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
            free(polygon.x);
            free(polygon.y);
        }
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

void draw(bool hasBorder=false, bool hasGuideLine=false) {
    clear();
    drawScene();
    drawBBox(&window, 255, 255, 255);
    if (hasBorder)
        drawBorder(255, 255, 255);
    if (hasGuideLine)
        drawGuideLine(255, 255, 0);
    update();
}

void init() {
    reset();
    drawBBox(&window, 255, 255, 255);
}

void destroy() {}

void onMouseDown(WORD x, WORD y) {
    y = HEIGHT - y;
    Point p = {x, y};
    if (!dirtyFlag) {
        std::vector<struct Point> points;
        points.push_back(p);
        scene.push_back(points);
        dirtyFlag = true;
    }
    else {
        scene.back().push_back(p);
    }
    if (mode == MODE_LINE) {
        dirtyFlag = scene.back().size() == 1;
    }
    draw(true);
}

void moveWindow(int deltaX, int deltaY) {
    window.xMin += deltaX;
    window.xMax += deltaX;
    window.yMin += deltaY;
    window.yMax += deltaY;
}

void onMouseMove(WORD x, WORD y) {
    y = HEIGHT - y;
    // 右键模式下移动窗口
    if (wndFlag) {
        int deltaX = x - (window.xMin + window.xMax) / 2;
        int deltaY = y - (window.yMin + window.yMax) / 2;
        moveWindow(deltaX, deltaY);
        draw();
    }
    // 左键模式下绘制直线，多边形辅助线
    else {
        lastX = x;
        lastY = y;
        if (dirtyFlag) draw(true, true);
        else draw();
    }
}

void onMouseUp(__attribute__((unused)) WORD x, __attribute__((unused)) WORD y) {}

void onRMouseDown(__attribute__((unused)) WORD x, __attribute__((unused)) WORD y) {
    wndFlag = true;
}

void onRMouseUp(__attribute__((unused)) WORD x, __attribute__((unused)) WORD y) {
    wndFlag = false;
}

void onKeyDown(WPARAM key) {
    if (key == VK_RETURN) {
        if (mode == MODE_POLY) dirtyFlag = false;
        draw();
    }
    else if (key == 'C') {
        clear();
        reset();
        update();
    }
    else if (key == '1') mode = MODE_LINE;
    else if (key == '2') mode = MODE_POLY;
    else {
        int dx = 0;
        int dy = 0;
        if (key == VK_UP) dy = 1;
        if (key == VK_DOWN) dy = -1;
        if (key == VK_LEFT) dx = -1;
        if (key == VK_RIGHT) dx = 1;
        if (dx != 0 || dy != 0) {
            moveWindow(dx, dy);
            draw();
        }
    }
}
