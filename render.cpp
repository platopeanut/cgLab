/**
* @Author: Peanut
* @Date: 2022/9/23 17:37
* @Email: platopeanut@gmail.com
*/

#include "./draw/line.h"
#include "./draw/polygon.h"
#include "./draw/circle.h"
#include "./display/win32.h"

Point2I* polygon;
// 记录直线起点位置
int lastX = -1, lastY = -1;
void drawLine();
void drawCircle();
void drawPolygon();

void init() {
    drawLine();
    drawCircle();
    drawPolygon();
}

void destroy() { delete polygon; }

void onMouseDown(WORD x, WORD y) {
    if (lastX == -1 && lastY == -1) {
        lastX = x;
        lastY = y;
        setPixel(x, HEIGHT - y, 255, 0, 0);
    } else {
        lineBresenham(lastX, HEIGHT - lastY, x, HEIGHT - y, 255, 0, 0);
        lastX = -1;
        lastY = -1;
    }
    update();
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

