#pragma once

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class CRI {
    public:
    void setConsoleTitle(string title) {

    }
    void setCursorVisible(bool visible) {

    }
    void putChar(int x, int y, char c, int colour = 0x07) {
    }
};

bool getKey(char n) {
    return false;
}

void sleep(int x) {
    std::this_thread::sleep_for(std::chrono::milliseconds(x));
}
