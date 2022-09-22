/**
*   实现"win32.h"中除生命周期回调函数（需要使用者自己实现）的其他函数
*/
#include "win32.h"

/**
 * 向终端打印字符串，由于win32中传统的cout(可能)不能使用，故用该函数代替之
 * @param text 所需打印的内容
 * 如果需要打印非字符串类型的数据，请使用std::to_string()将其转换为字符串类型
 * 例如：log("Hello" + std::to_string(123));
 */
void Log(const std::string& text) {
    WriteConsole(hConsole, text.c_str(), text.size(), nullptr, nullptr);
}

/**
 * 注册定时器，注册后每隔指定时间间隔会调用onTimer
 * @param id            定时器id
 * @param interval      定时器时间间隔（毫秒）
 */
void registerTimer(UINT id, UINT interval) {
    SetTimer(gHwnd, id, interval, nullptr);
}

/**
 * 释放定时器，一般在destroy中释放
 * @param id            定时器id
 */
void freeTimer(UINT id) {
    KillTimer(gHwnd, id);
}
