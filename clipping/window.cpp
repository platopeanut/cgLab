/**
* @Author: Peanut
* @Date: 2022/10/10 22:29
* @Email: platopeanut@gmail.com
*/

#include "window.h"

void drawBBox(const BBox_t *bbox, int r, int g, int b) {
    for (int i = bbox->xMin; i <= bbox->xMax; ++i) {
        setPixel(i, bbox->yMin, r, g, b);
        setPixel(i, bbox->yMax, r, g, b);
    }
    for (int i = bbox->yMin; i <= bbox->yMax; ++i) {
        setPixel(bbox->xMin, i, r, g, b);
        setPixel(bbox->xMax, i, r, g, b);
    }
}

bool pointClipping(const BBox_t *bbox, int x, int y) {
    return x >= bbox->xMin &&
           x <= bbox->xMax &&
           y >= bbox->yMin &&
           y <= bbox->yMax;
}
