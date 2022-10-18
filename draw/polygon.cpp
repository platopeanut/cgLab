/**
* @Author: Peanut
* @Date: 2022/9/22 21:25
* @Email: platopeanut@gmail.com
*/

#include "polygon.h"
#include "../display/win32.h"
#include <iostream>
#include <cmath>

Bucket::Bucket(int scanLine, Bucket *next): scanLine(scanLine), next(next), head(nullptr) {}

void Bucket::addTriple(double x, double deltaX, int yMax) {
    auto* triple = new Triple(x, deltaX, yMax);
    if (head == nullptr) {
        head = triple;
        return;
    }
    if (head->x > x || (head->x == x && head->deltaX > deltaX)) {
        triple->next = head;
        head = triple;
        return;
    }
    Triple* curr = head;
    // x从小到大
    while (curr->next != nullptr && curr->next->x < x)
        curr = curr->next;
    // deltaX从小到大
    while (curr->next != nullptr && curr->next->x == x && curr->next->deltaX < deltaX)
        curr = curr->next;
    triple->next = curr->next;
    curr->next = triple;
}

std::ostream &operator<<(std::ostream &os, const Bucket &bucket) {
    const Bucket* curr = &bucket;
    while (curr != nullptr) {
        os << curr->scanLine << *curr->head << std::endl;
        curr = curr->next;
    }
    return os;
}

Bucket::~Bucket() {
//    std::cout << "\nFree Bucket:" << scanLine;
    // 删除本身的triples
    Triple* currTri = head;
    while (head != nullptr) {
        head = head->next;
//        std::cout <<"->("<< currTri->x << ", " << currTri->deltaX << ", " << currTri->yMax << ")";
        delete currTri;
        currTri = head;
    }
}

void Bucket::freeBuckets(Bucket* head) {
    Bucket* curr = head;
    while (head != nullptr) {
        head = head->next;
        delete curr;
        curr = head;
    }
}

void findPolygonYRange(Point2I* polygon, int size, int &yMin, int &yMax) {
    yMin = yMax = polygon[0].y;
    for (int i = 1; i < size; ++i) {
        if (polygon[i].y < yMin) yMin = polygon[i].y;
        else if (polygon[i].y > yMax) yMax = polygon[i].y;
    }
}

void addNewEdge(Point2I *polygon, int size, Bucket *bucket) {
    for (int i = 0; i < size; ++i) {
        if (bucket->scanLine == polygon[i].y) {
            int lIndex = i == 0 ? size - 1: i - 1;
            int rIndex = i == size - 1 ? 0: i + 1;
            // add left edge
            if (polygon[lIndex].y > polygon[i].y) {
                double k = (polygon[lIndex].x - polygon[i].x) * 1.0 / (polygon[lIndex].y - polygon[i].y);
                bucket->addTriple(polygon[i].x, k, polygon[lIndex].y);
            }
            // add right edge
            if (polygon[rIndex].y > polygon[i].y) {
                double k = (polygon[rIndex].x - polygon[i].x) * 1.0 / (polygon[rIndex].y - polygon[i].y);
                bucket->addTriple(polygon[i].x, k, polygon[rIndex].y);
            }
        }
    }
}

Bucket *buildNET(Point2I *polygon, int size) {
    int yMin, yMax;
    findPolygonYRange(polygon, size, yMin, yMax);
    auto* NET = new Bucket(yMin);
    Bucket* tail = NET;
    addNewEdge(polygon, size, tail);
    for (int y = yMin + 1; y <= yMax; ++y) {
        auto curr = new Bucket(y);
        addNewEdge(polygon, size, curr);
        tail->next = curr;
        tail = tail->next;
    }
    return NET;
}

Triple::Triple(double x, double deltaX, int yMax, Triple *next) : x(x), deltaX(deltaX), yMax(yMax), next(next) {}

std::ostream &operator<<(std::ostream &os, const Triple &triple) {
    const Triple* curr = &triple;
    while (curr != nullptr) {
        os << "->(" << curr->x << ", " << curr->deltaX << ", " << curr->yMax << ")";
        curr = curr->next;
    }
    return os;
}

// 1. 求交    2. 排序
void buildAET(Bucket *NET) {
    Bucket* currBucket = NET;
    while (currBucket->next != nullptr) {
        Triple* currTri = currBucket->head;
        while (currTri != nullptr) {
            if (currBucket->next->scanLine < currTri->yMax) {
                currBucket->next->addTriple(currTri->x + currTri->deltaX, currTri->deltaX, currTri->yMax);
            }
            currTri = currTri->next;
        }
        currBucket = currBucket->next;
    }
}

// 3. 配对    4. 着色
void shaderAET(Bucket *AET, int r, int g, int b) {
    Bucket* currBucket = AET;
    while (currBucket != nullptr) {
        Triple* currTri = currBucket->head;
        while (currTri != nullptr && currTri->next != nullptr) {
            for (int x = (int)std::round(currTri->x); x < (int)std::round(currTri->next->x); ++x) {
                setPixel(x, currBucket->scanLine, r, g, b);
            }
            currTri = currTri->next->next;
        }
        currBucket = currBucket->next;
    }
}

Bucket* polygonScanLine(Point2I *polygon, int size, int r, int g, int b) {
    Bucket* NET = buildNET(polygon, size);
//    std::cout << "NET:" << std::endl;
//    std::cout << *NET;
    buildAET(NET);
//    std::cout << "AET:" << std::endl;
//    std::cout << *NET << std::endl;
    shaderAET(NET, r, g, b);
    return NET;
}

void getLRBorderFromAET(Bucket *AET, int x, int y, int* left, int* right) {
    Bucket* currBucket = AET;
    while (currBucket != nullptr && currBucket->scanLine != y)
        currBucket = currBucket->next;
    if (currBucket == nullptr) {
        *left = -1;
        *right = -1;
        return;
    }
    Triple* currTri = currBucket->head;
    while (currTri != nullptr && currTri->next != nullptr) {
        if (x >= currTri->x && x <= currTri->next->x)
            break;
        currTri = currTri->next->next;
    }
    if (currTri == nullptr) {
        *left = -1;
        *right = -1;
        return;
    }
    *left = (int) std::round(currTri->x);
    *right = (int) std::round(currTri->next->x);
}

void polygonSeedFilling(Bucket *AET, int seedX, int seedY, int r, int g, int b) {
//    std::cout << seedX << ", " << seedY << std::endl;
//    std::cout << *AET << std::endl;
    int left, right;
    getLRBorderFromAET(AET, seedX, seedY, &left, &right);
//    std::cout << left << " " << right << std::endl;
    if (left == -1 && right == -1) return;
    std::vector<Seg> stack;
    stack.emplace_back(left, right, seedY);
    while (!stack.empty()) {
        Seg seg = stack.back();
//        std::cout << seg.left << ", " << seg.right << ", " << seg.y << std::endl;
        stack.pop_back();
        int upR = -1, downR = -1;
        for (int x = seg.left; x <= seg.right; ++x) {
            setPixel(x, seg.y, r, g, b);
            if (x > upR) {
                getLRBorderFromAET(AET, x, seg.y - 1, &left, &right);
                if (left == -1 && right == -1) {
                    continue;
                }
                upR = right;
                int tmpR, tmpG, tmpB;
                getPixel(x, seg.y - 1, &tmpR, &tmpG, &tmpB);
                if (!(tmpR == r && tmpG == g && tmpB == b)) {
                    stack.emplace_back(left, right, seg.y - 1);
                }
            }
            if (x > downR) {
                getLRBorderFromAET(AET, x, seg.y + 1, &left, &right);
                if (left == -1 && right == -1) {
                    continue;
                }
                upR = right;
                int tmpR, tmpG, tmpB;
                getPixel(x, seg.y + 1, &tmpR, &tmpG, &tmpB);
                if (!(tmpR == r && tmpG == g && tmpB == b)) {
                    stack.emplace_back(left, right, seg.y + 1);
                }
            }
        }
    }
}

Seg::Seg(int left, int right, int y) : left(left), right(right), y(y) {}
