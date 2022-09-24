/**
* @Author: Peanut
* @Date: 2022/9/22 21:25
* @Email: platopeanut@gmail.com
*/

#ifndef CGLAB_POLYGON_H
#define CGLAB_POLYGON_H

#include <vector>
#include <ostream>
#include "../utils/Point.h"

// Polygon: Point2I[]

// 定义画多边形函数的函数指针类型
typedef void (*fpDrawPolygon) (Point2I* polygon, int size, int r, int g, int b);


class Triple {
public:
    double x;
    double deltaX;
    int yMax;
    Triple* next;

    Triple(double x, double deltaX, int yMax, Triple *next = nullptr);

    friend std::ostream &operator<<(std::ostream &os, const Triple &triple);
};

class Bucket {
public:
    int scanLine;
    Triple* head;
    Bucket* next;
    explicit Bucket(int scanLine, Bucket* next = nullptr);
    void addTriple(double x, double deltaX, int yMax);

    friend std::ostream &operator<<(std::ostream &os, const Bucket &bucket);

    ~Bucket();

    static void freeBuckets(Bucket* head);
};

void findPolygonYRange(Point2I* polygon, int size, int& yMin, int& yMax);

// 添加的时候就进行从小到大排序
void addNewEdge(Point2I* polygon, int size, Bucket* bucket);

Bucket* buildNET(Point2I* polygon, int size);

void buildAET(Bucket* NET);

void shaderAET(Bucket* AET, int r, int g, int b);

void polygonScanLine(Point2I* polygon, int size, int r, int g, int b);


#endif //CGLAB_POLYGON_H
