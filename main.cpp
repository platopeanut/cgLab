#include "./display/win32.h"
#include <cmath>

void init() {
    Log("Hello World!\n");
    registerTimer(1, 20);
}

void destroy() { freeTimer(1); }

void onKeyDown(WPARAM key) {}

void onMouseMove(WORD x, WORD y) {}

void onMouseDown(WORD x, WORD y) {}

void onMouseUp(WORD x, WORD y) {}

double iTime = 0;
void onTimer(WPARAM id) {
    if (id == 1) {
        for (int j = 0; j < HEIGHT; ++j) {
            for (int i = 0; i < WIDTH; ++i) {
                double r = 0.5 + 0.5 * cos(iTime + i * 1.0 / WIDTH);
                double g = 0.5 + 0.5 * cos(iTime + j * 1.0 / HEIGHT + 2);
                double b = 0.5 + 0.5 * cos(iTime + i * 1.0 / WIDTH + 4);
                setPixel(i, j, (int)(r * 255), (int)(g * 255), (int)(b * 255));
            }
        }
        update();
        iTime += 0.05;
    }
}