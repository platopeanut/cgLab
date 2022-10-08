#ifndef RENDER_WIN32_H
#define RENDER_WIN32_H

#include <windows.h>
#include <string>
#define WNDCLASS_NAME TEXT("Window Class")
#define WINDOW_NAME TEXT("Window Name")
#define WINDOW_POS_X CW_USEDEFAULT
#define WINDOW_POS_Y CW_USEDEFAULT
//#define WIDTH CW_USEDEFAULT
//#define HEIGHT CW_USEDEFAULT
#define WIDTH 600
#define HEIGHT 600

typedef unsigned char Byte;
extern HANDLE hConsole;
extern HWND gHwnd;
extern HDC hDC;
extern HDC hMemDC;
extern HBITMAP hBitmap;
extern HBITMAP hOldBitmap;
extern Byte* pFrameBuffer;

/* Tool Function (Implemented in utils.cpp) */

// i列j行
#define setPixel(i, j, r, g, b) \
        pFrameBuffer[((j)*WIDTH+(i))*4  ]=(b); \
        pFrameBuffer[((j)*WIDTH+(i))*4+1]=(g); \
        pFrameBuffer[((j)*WIDTH+(i))*4+2]=(r); \
        pFrameBuffer[((j)*WIDTH+(i))*4+3]=(0)  \

// 更新framebuffer
#define update() BitBlt(hDC, 0, 0, WIDTH, HEIGHT, hMemDC, 0, 0, SRCCOPY)
// 打印
void Log(const std::string& text);
// 注册定时器
void registerTimer(UINT id, UINT interval);
// 释放定时器
void freeTimer(UINT id);
// 设置窗口标题
void setWindowTitle(const std::string& title);
// 复制当前帧，需要自己提前分配空间，大小为：WIDTH * HEIGHT * 4(Byte)
void getFrame(Byte* pFrame);
// 设置当前帧，需要自己提前分配空间，大小为：WIDTH * HEIGHT * 4(Byte)
void setFrame(const Byte* pFrame);

/* External Interface (Implemented in main.cpp) */
void init();
void destroy();
void onKeyDown(WPARAM key);
void onMouseMove(WORD x, WORD y);
void onMouseDown(WORD x, WORD y);
void onMouseUp(WORD x, WORD y);
void onTimer(WPARAM id);


#endif //RENDER_WIN32_H
