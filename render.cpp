/**
* @Author: Peanut
* @Date: 2022/9/23 17:37
* @Email: platopeanut@gmail.com
*/

#include "./draw/line.h"
#include "./draw/polygon.h"
#include "./draw/circle.h"
#include "./display/win32.h"
#include <chrono>

Point2I* polygon;
std::string guideInfo = "Use 1,2,3 to switch line algorithm, Use Up,Down to change line width";
std::string lineNames[] = { "DDA", "Bresenham", "MidPoint" };
fpDrawLine lineFun[] = { lineDDA2, lineBresenham2, lineMidPoint2 };
int currIndex = 0;  // 当前算法索引
int lineWidth = 1;
// 记录直线起点位置
int lastX = -1, lastY = -1;
void drawLine();
void drawCircle();
void drawPolygon();
void drawLineWithWidth(int x1, int y1, int x2, int y2, int r, int g, int b);

void init() {
    setWindowTitle(guideInfo);
//    drawLine();
//    drawCircle();
//    drawPolygon();
}

void destroy() { delete polygon; }

void onMouseDown(WORD x, WORD y) {
    if (lastX == -1 && lastY == -1) {
        lastX = x;
        lastY = y;
        setPixel(x, HEIGHT - y, 255, 255, 255);
    } else {
        drawLineWithWidth(lastX, HEIGHT - lastY, x, HEIGHT - y, 255, 255, 255);
        lastX = -1;
        lastY = -1;
    }
    update();
}


void onKeyDown(WPARAM key) {
    if (key == '1' || key == '2' || key == '3') {
        currIndex = (int) (key - '1');
    } else if (key == VK_UP) {
        lineWidth ++;
    } else if (key == VK_DOWN) {
        if (lineWidth > 1) lineWidth --;
    } else {
        setWindowTitle(guideInfo);
        return;
    }
    setWindowTitle("Curr: " + lineNames[currIndex] + " | Line Width: " + std::to_string(lineWidth));
}

void drawLine() {
    fpDrawLine fpLines[] = {
            lineSimple,
            lineSimple2,
            lineDDA,
            lineDDA2,
            lineBresenham,
            lineBresenham2,
            lineMidPoint,
            lineMidPoint2,
    };
    // k < 1
    int fromX1 = 50, fromY1 = 100, toX1 = 300, toY1 = 200;
    // k > 1
    int fromX2 = 50, fromY2 = 100, toX2 = 100, toY2 = 300;
    int incrX = 20;
    for (int i = 0; i < sizeof(fpLines) / sizeof(fpDrawLine); ++i) {
        fpLines[i](fromX1 + incrX * i, fromY1, toX1 + incrX * i, toY1, 255, 0, 0);
        fpLines[i](fromX2 + incrX * i, fromY2, toX2 + incrX * i, toY2, 255, 0, 0);
    }
}

void drawCircle() {
    fpDrawCircle fpCircles[] = {
            circleSimple,
            circleSimple2,
            circleMidPoint,
            circleMidPoint2
    };

    int cx = 150, cy = 450, radius=100;
    int r = 0, g = 255, b = 0;
    int offset = 30;

    for (int i = 0; i < sizeof(fpCircles) / sizeof(fpDrawCircle); ++i) {
        fpCircles[i](cx + offset * i, cy, radius, r, g, b);
    }
}

void drawPolygon() {
    fpDrawPolygon fpPolygon = polygonScanLine;

    int size = 6;
    polygon = new Point2I[size];
    int i = 0;
    polygon[i++].setXY(2, 2);
    polygon[i++].setXY(5, 1);
    polygon[i++].setXY(11, 3);
    polygon[i++].setXY(11, 8);
    polygon[i++].setXY(5, 5);
    polygon[i  ].setXY(2, 7);

//    int size = 4;
//    polygon = new Point2I[size];
//    int i = 0;
//    polygon[i++].setXY(1, 1);
//    polygon[i++].setXY(10, 1);
//    polygon[i++].setXY(10, 10);
//    polygon[i++].setXY(1, 10);

    // transform
    for (i = 0; i < size; ++i) {
        polygon[i].x *= 20;
        polygon[i].x += 300;
        polygon[i].y *= 20;
        polygon[i].y += 200;
    }

    fpPolygon(polygon, size, 0, 255, 255);
}

// 根据lineWidth画线
void drawLineWithWidth(int x1, int y1, int x2, int y2, int r, int g, int b) {
    // 没有边界检测
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < lineWidth; ++i) {
        lineFun[currIndex](x1-lineWidth/2+i, y1, x2-lineWidth/2+i, y2, r, g, b);
    }
    auto finish = std::chrono::high_resolution_clock::now();
    auto cost = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
    setWindowTitle("Curr: " + lineNames[currIndex]
            + " | Line Width: " + std::to_string(lineWidth)
            + "| Cost: " + std::to_string((double) cost / 1e+6) + "ms");
}
