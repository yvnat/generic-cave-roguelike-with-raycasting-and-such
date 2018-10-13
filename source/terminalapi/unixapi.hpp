#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <curses.h>

using namespace std;

class CRI {
    public:
    void setConsoleTitle(string title) {

    }
    void setCursorVisible(bool visible) {
        curs_set((int)visible);
    }
    void putChar(int x, int y, char c, int colour = 0x07) {

    }
};

inline bool getKey(char n) {
    return false;
}

inline void sleep(int x) {
    std::this_thread::sleep_for(std::chrono::milliseconds(x));
}
