# Computer Graphics Lab in CQU

## Branch Info

chp2: Draw Primitive

## Implemented Algorithms

- Line
    - [x] Simple
    - [x] Simple2
    - [x] DDA
    - [x] DDA2
    - [x] Bresenham
    - [x] Bresenham2
    - [x] MidPoint
    - [x] MidPoint2
- Circle
    - [x] Simple
    - [x] Simple2
    - [x] MidPoint
    - [x] MidPoint2
- Polygon
    - [x] Scan-Line
    - [ ] 边缘填充算法
      - [ ] 以扫描线为中心
      - [ ] 以边为中心
    - [ ] 边界标志算法
- 区域填充算法
  - [ ] 4连通
  - [ ] 8连通
  - [ ] 种子填充算法
- Antialiasing
  - [x] Unweighted Area Sampling
  - [x] Weighted Area Sampling

## General Line Drawing Algorithm
In the line drawing algorithm, some algorithms are limited by _x1 >= x0_ and the range of **K**(_0 <= k <= 1_).
Inserting **transform** operations before and after the basic line drawing algorithm can achieve general line drawing.
The advantage of this is that the original code is minimally intrusive and easy to replace, rather than using cumbersome conditionals.
Its downside is that performance will be reduced.
### Restriction: x1 >= x0
Swap the left and right endpoints at the beginning of the line drawing algorithm to ensure that _x1 >= x0_;
Swap operations use inline functions to improve performance
~~~cpp
inline void swap(int& a, int& b) {
    int tmp = a;
    a = b;
    b = tmp;
}

void lineDrawingAlgorithm(int x0, int y0, int x1, int y1, int r, int g, int b) {
    if (x0 > x1) {
        swap(x0, x1);
        swap(y0, y1);
    }
    // ...
}
~~~

### Restriction: 0 <= k <= 1
Note: The operation _before_ is the **opposite** of the operation _after_.
#### Before drawing line
~~~cpp
void lineDrawingAlgorithm(int x0, int y0, int x1, int y1, int r, int g, int b) {
    // swap operation
    bool negativeFlag = false;      // when k < 0
    if (y1 < y0) {
        negativeFlag = true;
        y1 = 2 * y0 - y1;
    }
    bool steepFlag = false;         // when abs(k) > 1
    if (y1 - y0 > x1 - x0) {
        steepFlag = true;
        int tmpX1 = x1;
        x1 = y1 - y0 + x0;
        y1 = tmpX1 - x0 + y0;
    }
    // ...
}
~~~

#### After drawing line
Replace the **original** code with the **present** code.
~~~cpp
// Original
setPixel(x, y, r, g, b);

// Present
int finalX = x, finalY = y;
if (steepFlag) {
    int tmpFinalX = finalX;
    finalX = finalY - y0 + x0;
    finalY = tmpFinalX - x0 + y0;
}
if (negativeFlag) {
    finalY = 2 * y0 - finalY;
}
setPixel(finalX, finalY, r, g, b);
~~~
