#include "win32.h"

HANDLE hConsole;
HWND gHwnd;
HDC hDC;
HDC hMemDC;
HBITMAP hBitmap;
HBITMAP hOldBitmap;
Byte* pFrameBuffer;


LRESULT CALLBACK
WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
    // Register the window class.
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = WNDCLASS_NAME;
    RegisterClass(&wc);
    // Create the window.
    gHwnd = CreateWindowEx(
            0,                                     // Optional window styles.
            WNDCLASS_NAME,                       // Window class
            WINDOW_NAME,                      // Window text
            WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,    // Window style: WS_THICKFRAME(Fixed Window Size)
            // Size and position
            WINDOW_POS_X, WINDOW_POS_Y, WIDTH, HEIGHT,
            nullptr,                                      // Parent window
            nullptr,                                          // Menu
            hInstance,                                            // Instance handle
            nullptr                                          // Additional application data
    );
    if (gHwnd == nullptr) {
        // Create Window Failed!
        MessageBox(nullptr, TEXT(
                           "This program requires Windows NT!"),
                   WINDOW_NAME, MB_ICONERROR);
        return -1;
    }
    ShowWindow(gHwnd, nCmdShow);
    // Run the Message Loop.
    MSG msg = {};
    init();
    update();
    while (GetMessage(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK
WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            // Attach Console
            AllocConsole();
            hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            // DC
            hDC = GetDC(hwnd);
            hMemDC = CreateCompatibleDC(hDC);
            LPVOID pImgData;
            BITMAPINFO bitmapInfo = {{sizeof(BITMAPINFOHEADER), WIDTH, HEIGHT, 1, 32, BI_RGB,
                                      WIDTH * HEIGHT * 4, 0, 0, 0, 0 }  };
            hBitmap = CreateDIBSection(
                    hMemDC,
                    &bitmapInfo,
                    DIB_RGB_COLORS,
                    &pImgData,
                    nullptr,0);
            pFrameBuffer = (Byte*) pImgData;
            hOldBitmap = (HBITMAP) SelectObject(hMemDC, hBitmap);
            memset(pFrameBuffer, 0, WIDTH*HEIGHT*4);
        }
            break;
        case WM_DESTROY: {
            // API
            destroy();
            // Detach Console
            FreeConsole();
            // Bitmap
            SelectObject(hMemDC, hOldBitmap);
            hOldBitmap = nullptr;
            DeleteObject(hBitmap);
            // Free DC
            ReleaseDC(hwnd, hDC);
            ReleaseDC(hwnd, hMemDC);
            // End
            PostQuitMessage(0);
        }
            break;
        case WM_KEYDOWN: {
            onKeyDown(wParam);
        }
            break;
        case WM_MOUSEMOVE: {
            onMouseMove(LOWORD(lParam), HIWORD(lParam));
        }
            break;
        case WM_LBUTTONDOWN: {
            onMouseDown(LOWORD(lParam), HIWORD(lParam));
        }
            break;
        case WM_LBUTTONUP: {
            onMouseUp(LOWORD(lParam), HIWORD(lParam));
        }
            break;
        case WM_RBUTTONDOWN: {
            onRMouseDown(LOWORD(lParam), HIWORD(lParam));
        }
            break;
        case WM_RBUTTONUP: {
            onRMouseUp(LOWORD(lParam), HIWORD(lParam));
        }
            break;
        case WM_TIMER: {
            onTimer(wParam);
        }
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}