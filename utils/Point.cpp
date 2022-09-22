/**
* @Author: Peanut
* @Date: 2022/9/22 21:40
* @Email: platopeanut@gmail.com
*/

#include "Point.h"

Point2I::Point2I(int x, int y) : x(x), y(y) {}

void Point2I::setXY(int _x, int _y) {
    this->x = _x;
    this->y = _y;
}
