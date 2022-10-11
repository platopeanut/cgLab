/**
* @Author: Peanut
* @Date: 2022/10/11 0:13
* @Email: platopeanut@gmail.com
*/

#include "lineClipping.h"

rgCode_t regionEncode(const BBox_t *bbox, int x, int y) {
    rgCode_t code = 0b0000;
    if (y > bbox->yMax) code |= 0b1000;
    if (y < bbox->yMin) code |= 0b0100;
    if (x > bbox->xMax) code |= 0b0010;
    if (x < bbox->xMin) code |= 0b0001;
    return code;
}

void lineClippingCohenSutherland(const BBox_t *bbox, const Line_t *srcLine, Line_t *dstLine) {
    dstLine->x1 = srcLine->x1;
    dstLine->y1 = srcLine->y1;
    dstLine->x2 = srcLine->x2;
    dstLine->y2 = srcLine->y2;
    rgCode_t c1 = regionEncode(bbox, dstLine->x1, dstLine->y1);
    rgCode_t c2 = regionEncode(bbox, dstLine->x2, dstLine->y2);
    while (true) {
        while (c1 != 0) {
            if ((c1 & c2) != 0) {
                dstLine->x1 = -1;   // set invalid
                return;
            }
            if ((c1 & 0b0001) != 0) {
                dstLine->y1 += (dstLine->y2 - dstLine->y1)
                               * (bbox->xMin - dstLine->x1)
                               / (dstLine->x2 - dstLine->x1);
                dstLine->x1 = bbox->xMin;
                c1 = regionEncode(bbox, dstLine->x1, dstLine->y1);
            }
            else if ((c1 & 0b0010) != 0) {
                dstLine->y1 += (dstLine->y2 - dstLine->y1)
                               * (bbox->xMax - dstLine->x1)
                               / (dstLine->x2 - dstLine->x1);
                dstLine->x1 = bbox->xMax;
                c1 = regionEncode(bbox, dstLine->x1, dstLine->y1);
            }
            else if ((c1 & 0b0100) != 0) {
                dstLine->x1 += (dstLine->x2 - dstLine->x1)
                               * (bbox->yMin - dstLine->y1)
                               / (dstLine->y2 - dstLine->y1);
                dstLine->y1 = bbox->yMin;
                c1 = regionEncode(bbox, dstLine->x1, dstLine->y1);
            }
            else {
                dstLine->x1 += (dstLine->x2 - dstLine->x1)
                               * (bbox->yMax - dstLine->y1)
                               / (dstLine->y2 - dstLine->y1);
                dstLine->y1 = bbox->yMax;
                c1 = regionEncode(bbox, dstLine->x1, dstLine->y1);
            }
        }
        if (c2 == 0) return;
        // swap x1, x2
        int tmp1 = dstLine->x1;
        dstLine->x1 = dstLine->x2;
        dstLine->x2 = tmp1;
        // swap y1, y2
        tmp1 = dstLine->y1;
        dstLine->y1 = dstLine->y2;
        dstLine->y2 = tmp1;
        // swap c1, c2
        rgCode_t tmp2 = c1;
        c1 = c2;
        c2 = tmp2;
    }
}
