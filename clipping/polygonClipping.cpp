/**
* @Author: Peanut
* @Date: 2022/10/11 20:58
* @Email: platopeanut@gmail.com
*/

#include "polygonClipping.h"
#include "lineClipping.h"
#include <vector>
#include <iostream>

// 0 ~ 3                    window
// 4 + 0 ~ 4 + size - 1     polygon
// 4 + size ~ ..            intersection

int indexOf(const std::vector<int>& v, int value) {
    for (int i = 0; i < v.size(); ++i)
        if (v[i] == value) return i;
    return -1;
}

Polygon_t *polygonClippingWeilerAthenton(const BBox_t *bbox, const Polygon_t *polygon, int *returnSize) {
    int bboxX[4] = {bbox->xMin, bbox->xMin, bbox->xMax, bbox->xMax};
    int bboxY[4] = {bbox->yMin, bbox->yMax, bbox->yMax, bbox->yMin};
    std::vector<struct Point> intersect;
    // intersect的标记数组，用于记录"入点和出点"
    // true为入点 false为出点
    std::vector<bool> marks;
    bool mark = !pointClipping(bbox, polygon->x[0], polygon->y[0]);
    std::vector<int> polygonLink;
    std::vector<int> windowLink = {0, 1, 2, 3};
    for (int i = 0; i < polygon->size; ++i) {
        Line_t srcLine = {
                polygon->x[i],
                polygon->y[i],
                polygon->x[(i+1) % polygon->size],
                polygon->y[(i+1) % polygon->size]
        };
        Line_t dstLine;
        lineClippingCohenSutherland(bbox, &srcLine, &dstLine);
        polygonLink.push_back(4 + i);
        if (dstLine.data[0] != -1) {
            for (int j = 0; j < 2; ++j) {
                if (srcLine.data[j*2] == dstLine.data[j*2] &&
                    srcLine.data[j*2+1] == dstLine.data[j*2+1])
                    continue;
                struct Point point = {dstLine.data[j*2], dstLine.data[j*2+1]};
                polygonLink.push_back(4 + polygon->size + (int) intersect.size());
                // 插入到windowLink中
                // 1. 判断在哪个i-j之间，i，j都属于[0,3]，且i < j
                // 2. 依次遍历windowLink中ij之间的交点，满足条件后插入
                int idx = 0;
                // 0-1
                if (point.x == bbox->xMin) {
                    for (; idx < windowLink.size(); ++idx)
                        if (windowLink[idx] == 0) break;
                    for (idx ++; windowLink[idx] != 1; ++idx) {
                        if (windowLink[idx] != 1 && intersect[windowLink[idx]-(4+polygon->size)].y >= point.y)
                            break;
                    }
                }
                // 1-2
                else if (point.y == bbox->yMax) {
                    for (; idx < windowLink.size(); ++idx)
                        if (windowLink[idx] == 1) break;
                    for (idx ++; windowLink[idx] != 2; ++idx) {
                        if (windowLink[idx] != 2 && intersect[windowLink[idx]-(4+polygon->size)].x >= point.x)
                            break;
                    }
                }
                // 2-3
                else if (point.x == bbox->xMax) {
                    for (; idx < windowLink.size(); ++idx)
                        if (windowLink[idx] == 2) break;
                    for (idx ++; windowLink[idx] != 3; ++idx) {
                        if (windowLink[idx] != 3 && intersect[windowLink[idx]-(4+polygon->size)].y <= point.y)
                            break;
                    }
                }
                // 3-0
                else if (point.y == bbox->yMin) {
                    for (; idx < windowLink.size(); ++idx)
                        if (windowLink[idx] == 3) break;
                    for (idx ++; idx < windowLink.size(); ++idx) {
                        if (intersect[windowLink[idx]-(4+polygon->size)].x <= point.x)
                            break;
                    }
                }
                windowLink.insert(windowLink.begin()+idx, 4 + polygon->size + (int) intersect.size());
                intersect.push_back(point);
                marks.push_back(mark);
                mark = !mark;
            }
        }
    }
//    std::cout << "\nPolygon Link: ";
//    for (const auto &item: polygonLink)
//        std::cout << item << ", ";
//    std::cout << "\nWindow Link: ";
//    for (const auto &item: windowLink)
//        std::cout << item << ", ";
//    std::cout << "\nMarks: ";
//    for (const auto &item: marks)
//        std::cout << item << ", ";
    std::vector<std::vector<int>> indexLoops;
    std::vector<int>* links[2] = {&polygonLink, &windowLink};
    while (true) {
        // 当所有入点都被遍历则跳出循环
        bool flag = true;
        int i = 0;
        for (; i < marks.size(); ++i) {
            if (marks[i]) {
                flag = false;
                break;
            }
        }
        if (flag) break;
        std::vector<int> indexLoop;
        int head = i + 4 + polygon->size;
        indexLoop.push_back(head);
        marks[i] = false;
        int mode = 0;   // 用于在polygonLink与windowLink之间切换
        int idx = indexOf(*links[0], head);
        idx = (idx + 1) % (int) links[0]->size();
        while (true) {
            if (mode == 1 && links[mode]->at(idx) == head) break;
            while (links[mode]->at(idx) < 4 + polygon->size) {
                indexLoop.push_back(links[mode]->at(idx));
                idx = (idx + 1) % (int) links[mode]->size();
            }
            if (mode == 1 && links[mode]->at(idx) == head) break;
            indexLoop.push_back(links[mode]->at(idx));
            marks[links[mode]->at(idx) - 4 - polygon->size] = false;
            idx = indexOf(*links[(mode + 1) % 2], links[mode]->at(idx));
            mode = (mode + 1) % 2;
            idx = (idx + 1) % (int) links[mode]->size();
        }
        indexLoops.push_back(indexLoop);
    }
//    for (const auto &loop: indexLoops) {
//        std::cout << "\nloop: ";
//        for (const auto &item: loop)
//            std::cout << item << ", ";
//    }
    *returnSize = (int) indexLoops.size();
    auto res = (Polygon_t*) malloc(sizeof(Polygon_t) * (*returnSize));
    for (int i = 0; i < (*returnSize); ++i) {
        res[i].size = (int) indexLoops[i].size();
        res[i].x = (int*) malloc(sizeof(int) * res[i].size);
        res[i].y = (int*) malloc(sizeof(int) * res[i].size);
        for (int j = 0; j < res[i].size; ++j) {
            int idx = indexLoops[i][j];
            if (idx < 4) {
                res[i].x[j] = bboxX[idx];
                res[i].y[j] = bboxY[idx];
            } else if (idx < 4 + polygon->size) {
                res[i].x[j] = polygon->x[idx-4];
                res[i].y[j] = polygon->y[idx-4];
            } else {
                res[i].x[j] = intersect[idx-4-polygon->size].x;
                res[i].y[j] = intersect[idx-4-polygon->size].y;
            }
        }
    }
    return res;
}

void freePolygons(Polygon_t *polygons, int size) {
    for (int i = 0; i < size; ++i) {
        free(polygons[i].x);
        free(polygons[i].y);
    }
    free(polygons);
}
