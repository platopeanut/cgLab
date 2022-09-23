/**
* @Author: Peanut
* @Date: 2022/9/23 17:37
* @Email: platopeanut@gmail.com
*/

#include "./draw/line.h"
#include "./draw/polygon.h"

Point2I* polygon;
void drawLine();
void drawPolygon();

void init() {
    drawLine();
    drawPolygon();
}

void destroy() { delete polygon; }

void drawLine() {
    // k > 1
//    int fromX = 50, fromY = 100, toX = 100, toY = 400;
    // k < 1
    int fromX = 50, fromY = 100, toX = 300, toY = 200;
    int incrX = 20, cnt=0;
    lineSimple(fromX + incrX * cnt, fromY, toX + incrX * cnt, toY, 255, 0, 0);
    cnt++;
    lineSimple2(fromX + incrX * cnt, fromY, toX + incrX * cnt, toY, 255, 0, 0);
    cnt++;
    lineDDA(fromX + incrX * cnt, fromY, toX + incrX * cnt, toY, 255, 0, 0);
    cnt++;
    lineDDA2(fromX + incrX * cnt, fromY, toX + incrX * cnt, toY, 255, 0, 0);
    cnt++;
    lineBresenham(fromX + incrX * cnt, fromY, toX + incrX * cnt, toY, 255, 0, 0);
    cnt++;
    lineBresenham2(fromX + incrX * cnt, fromY, toX + incrX * cnt, toY, 255, 0, 0);
}

void drawPolygon() {
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
    polygonScanLine(polygon, size, 0, 255, 255);
}
