/**
* @Author: Peanut
* @Date: 2022/9/22 21:40
* @Email: platopeanut@gmail.com
*/

#ifndef CGLAB_POINT_H
#define CGLAB_POINT_H


class Point2I {
public:
    int x;
    int y;

    explicit Point2I(int x=0, int y=0);
    void setXY(int x, int y);
};


#endif //CGLAB_POINT_H
