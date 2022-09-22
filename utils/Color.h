/**
* @Author: Peanut
* @Date: 2022/9/22 19:28
* @Email: platopeanut@gmail.com
*/

#ifndef CGLAB_COLOR_H
#define CGLAB_COLOR_H


class Color {
public:
    float r;    // [0, 1]
    float g;    // [0, 1]
    float b;    // [0, 1]

    Color(float r, float g, float b);
    void toInt(int& ir, int& ig, int& ib) const;
};


#endif //CGLAB_COLOR_H
