/**
* @Author: Peanut
* @Date: 2022/9/23 17:37
* @Email: platopeanut@gmail.com
*/

#include "./display/win32.h"
#include "./draw/others.h"
#include "./draw/circle.h"

#define min(a, b) (a) > (b) ? (b) : (a)

int twoPoints[4] = { -1, -1, -1, -1 };
int adjustPoints[4] = {-1, -1, -1, -1};
bool lock = false;
Byte* pFrame;

// 绘制模式：1: ellipseMidPoint, 2: circleMidPoint2, 3: circleBresenham
int mode = 1;


void adjust() {
    adjustPoints[0] = twoPoints[0];
    adjustPoints[1] = HEIGHT - twoPoints[1];
    adjustPoints[2] = twoPoints[2];
    adjustPoints[3] = HEIGHT - twoPoints[3];
    if (adjustPoints[0] > adjustPoints[2]) {
        int tmp = adjustPoints[0];
        adjustPoints[0] = adjustPoints[2];
        adjustPoints[2] = tmp;
    }
    if (adjustPoints[1] < adjustPoints[3]) {
        int tmp = adjustPoints[1];
        adjustPoints[1] = adjustPoints[3];
        adjustPoints[3] = tmp;
    }
}

void init() {
    setWindowTitle("mode:1 ellipse MidPoint");
    pFrame = (Byte*) malloc(sizeof(Byte) * WIDTH * HEIGHT * 4);
}

void destroy() {
    free(pFrame);
}

void onMouseDown(WORD x, WORD y) {
    if (twoPoints[0] == -1 && twoPoints[1] == -1) {
        twoPoints[0] = (int) x;
        twoPoints[1] = (int) y;
        lock = true;
        getFrame(pFrame);
    }
}

void onMouseMove(WORD x, WORD y) {
    if (!lock) return;
    setFrame(pFrame);
    twoPoints[2] = x;
    twoPoints[3] = y;
    adjust();
    squareSimple(adjustPoints[0], adjustPoints[1], adjustPoints[2], adjustPoints[3], 255, 0, 0);
    update();
}

void onMouseUp(WORD x, WORD y) {
    twoPoints[2] = x;
    twoPoints[3] = y;
    adjust();
    setFrame(pFrame);
    int cx = (adjustPoints[0] + adjustPoints[2]) / 2;
    int cy = (adjustPoints[1] + adjustPoints[3]) / 2;
    int a = (adjustPoints[2] - adjustPoints[0]) / 2;
    int b = (adjustPoints[1] - adjustPoints[3]) / 2;
    if (mode == 1)
        ellipseMidPoint(cx, cy, a, b, 255, 0, 0);
    else if (mode == 2)
        circleMidPoint2(cx, cy, min(a, b), 255, 0, 0);
    else if (mode == 3)
        circleBresenham(cx, cy, min(a, b), 255, 0, 0);
    update();
    for (int & twoPoint : twoPoints) {
        twoPoint = -1;
    }
    lock = false;
}

void onKeyDown(WPARAM key) {
    if (key == '1') {
        mode = 1;
        setWindowTitle("mode:1 ellipse MidPoint");
    }
    else if (key == '2') {
        mode = 2;
        setWindowTitle("mode:2 circle MidPoint");
    }
    else if (key == '3') {
        mode = 3;
        setWindowTitle("mode:3 circle Bresenham");
    }
}
