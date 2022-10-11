/**
* @Author: Peanut
* @Date: 2022/9/23 17:37
* @Email: platopeanut@gmail.com
*/

#include "./display/win32.h"
#include "./draw/line.h"
#include "./clipping/window.h"
#include "./clipping/lineClipping.h"
#include <iostream>

// TODO: 添加事件：右键"down-move-up"可以移动window

BBox_t window = {150, 450, 200, 400};
Line_t line = {-1, -1, -1, -1};

void init() {
    drawBBox(&window, 255, 255, 255);
}

void destroy() {}

void onMouseDown(WORD x, WORD y) {
    y = HEIGHT - y;
    std::cout << pointClipping(&window, x, y) << " |" << x << "," << y << std::endl;
    if (line.x1 == -1) {
        line.x1 = x;
        line.y1 = y;
    } else {
        line.x2 = x;
        line.y2 = y;
        Line_t cutLine;
        lineClippingCohenSutherland(&window, &line, &cutLine);
        if (cutLine.x1 != -1) {
            if (abs(cutLine.x1 - line.x1) > abs(cutLine.x2 - line.x1)) {
                // swap
                int tmp = cutLine.x1;
                cutLine.x1 = cutLine.x2;
                cutLine.x2 = tmp;
                tmp = cutLine.y1;
                cutLine.y1 = cutLine.y2;
                cutLine.y2 = tmp;
            }
            lineDDA2(line.x1, line.y1, cutLine.x1, cutLine.y1, 255, 0, 0);
            lineDDA2(cutLine.x1, cutLine.y1, cutLine.x2, cutLine.y2, 0, 255, 0);
            lineDDA2(cutLine.x2, cutLine.y2, line.x2, line.y2, 255, 0, 0);
        } else {
            lineDDA2(line.x1, line.y1, line.x2, line.y2, 255, 0, 0);
        }
        line.x1 = -1;
        update();
    }
}

void onMouseMove(WORD x, WORD y) {}

void onMouseUp(WORD x, WORD y) {}

void onKeyDown(WPARAM key) {}
