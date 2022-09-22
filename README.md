# Computer Graphics Lab in CQU

## Env

- Windows
- MinGW
- CMake

## Tool Function

- setPixel
  - 向i行j列像素点设置rgb
- update
  - 更新framebuffer
- Log
  - 打印
- registerTimer
  - 注册定时器
- freeTimer
  - 释放定时器

## Lifecycle Callback Function

### init

该函数在窗口创建时被调用，主要完成以下任务:
- 资源初始化
  - 使用者自己所需的资源
  - 注册定时器（如果需要）
- 初始图像绘制

### destroy

该函数在窗口销毁前被调用，主要完成以下任务:
- 资源释放
  - 使用者自己的资源
  - 释放定时器（如果需要）

### onKeyDown

该函数在键盘下落时被调用，传入的key表示当前用户所按下的键
具体键位请参阅[虚拟键代码](https://learn.microsoft.com/zh-cn/windows/win32/inputdev/virtual-key-codes)
- 使用示例
~~~cpp
void onKeyDown(WPARAM key) {
    // 当按下'A'或Enter键时将屏幕设置为红色
    if (key == 'A' || key == VK_RETURN) {
        for (int j = 0; j < HEIGHT; ++j) {
            for (int i = 0; i < WIDTH; ++i) {
                setPixel(i, j, 255, 0, 0);
            }
        }
        update();
    }
}
~~~

### onMouse*

该函数在用户鼠标**左键**被down，up，move时调用，传入的x，y分别表示当前鼠标的坐标（屏幕左上方为（0，0））
- 使用示例
~~~cpp
void onMouseDown(WORD x, WORD y) {
    // 打印鼠标当前位置
    log("MouseDown:" + std::to_string(x) + ", " + std::to_string(y) + "\n");
}
~~~

### onTimer

该函数使用前，需要用户在init中**注册**对应的定时器。该函数在定时器触发了对应的时间间隔时被调用，传入的id表示当前触发的定时器id
BTW，请在destroy中**释放**对应的定时器。

- 使用示例
~~~cpp
void init() {
    // 注册定时器，id=1，时间间隔为1000ms
    registerTimer(1, 1000);
}

void destroy() {
    // 释放id=1的定时器
    freeTimer(1);
}

void onTimer(WPARAM id) {
    // 处理定时器id=1
    if (id == 1) {
        log("Timer 1\n");
    }
}
~~~