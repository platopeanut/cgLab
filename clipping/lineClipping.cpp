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
    for (int i = 0; i < 4; ++i) {
        dstLine->data[i] = srcLine->data[i];
    }
    rgCode_t codes[2] = {
            regionEncode(bbox, dstLine->data[0], dstLine->data[1]),
            regionEncode(bbox, dstLine->data[2], dstLine->data[3])
    };
    for (int i = 0; i < 2; ++i) {
        while (codes[i] != 0) {
            if ((codes[0] & codes[1]) != 0) {
                dstLine->data[0] = -1;   // set invalid
                return;
            }
            if ((codes[i] & 0b0001) != 0) {
                dstLine->data[i*2+1] += (dstLine->data[3] - dstLine->data[1])
                               * (bbox->xMin - dstLine->data[i*2])
                               / (dstLine->data[2] - dstLine->data[0]);
                dstLine->data[i*2] = bbox->xMin;
            }
            else if ((codes[i] & 0b0010) != 0) {
                dstLine->data[i*2+1] += (dstLine->data[3] - dstLine->data[1])
                               * (bbox->xMax - dstLine->data[i*2])
                               / (dstLine->data[2] - dstLine->data[0]);
                dstLine->data[i*2] = bbox->xMax;
            }
            else if ((codes[i] & 0b0100) != 0) {
                dstLine->data[i*2] += (dstLine->data[2] - dstLine->data[0])
                               * (bbox->yMin - dstLine->data[i*2+1])
                               / (dstLine->data[3] - dstLine->data[1]);
                dstLine->data[i*2+1] = bbox->yMin;
            }
            else {
                dstLine->data[i*2] += (dstLine->data[2] - dstLine->data[0])
                               * (bbox->yMax - dstLine->data[i*2+1])
                               / (dstLine->data[3] - dstLine->data[1]);
                dstLine->data[i*2+1] = bbox->yMax;
            }
            codes[i] = regionEncode(bbox, dstLine->data[i*2], dstLine->data[i*2+1]);
        }
    }
}
