/**
* @Author: Peanut
* @Date: 2022/10/11 20:58
* @Email: platopeanut@gmail.com
*/

#ifndef CGLAB_POLYGONCLIPPING_H
#define CGLAB_POLYGONCLIPPING_H

#include "./window.h"

struct Point {
    int x;
    int y;
};

/**
 * @param bbox          指定的窗口
 * @param polygon       待裁剪的多边形
 * @param returnSize    输出的多边形的个数
 * @return              输出的多边形的数组地址
 * Note: 调用者应该手动free返回的数组
 */
Polygon_t* polygonClippingWeilerAthenton(const BBox_t* bbox, const Polygon_t* polygon, int* returnSize);

// 释放polygon数组
void freePolygons(Polygon_t* polygons, int size);

#endif //CGLAB_POLYGONCLIPPING_H
