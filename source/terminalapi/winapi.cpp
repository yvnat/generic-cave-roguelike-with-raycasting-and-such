#include "windows.h"
#include <string>	//this is good because it prevents the error
#include <iostream>

using namespace std;

class CRI {	//console renovation initiative (pronounced "cry")
	public:
	HANDLE STDHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	void setConsoleTitle(string title) {
		SetConsoleTitle(title.c_str());
	}
	void setCursorVisible(bool showFlag) {\
		CONSOLE_CURSOR_INFO cursorInfo;
		GetConsoleCursorInfo(STDHandle, &cursorInfo);
		cursorInfo.bVisible = showFlag; // set the cursor visibility
		SetConsoleCursorInfo(STDHandle, &cursorInfo);
	}
	void putChar(int x, int y, char c, int colour = 0x07) {
		SetConsoleCursorPosition(STDHandle, {(SHORT)x, (SHORT)y});
		SetConsoleTextAttribute(STDHandle, (WORD)colour);
		cout << c;
	}
};

bool getKey(char n) {
    return GetKeyState(n) & 0x8000;
}

void sleep(int x) {
	Sleep(x);
}