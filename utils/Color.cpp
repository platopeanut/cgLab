/**
* @Author: Peanut
* @Date: 2022/9/22 19:28
* @Email: platopeanut@gmail.com
*/

#include "Color.h"

Color::Color(float r, float g, float b) : r(r), g(g), b(b) {}

void Color::toInt(int &ir, int &ig, int &ib) const {
    ir = (int) (r * 255);
    ig = (int) (g * 255);
    ib = (int) (b * 255);
}
