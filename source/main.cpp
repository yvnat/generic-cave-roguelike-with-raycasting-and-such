/*The different color codes are

0   BLACK
1   BLUE
2   GREEN
3   CYAN
4   RED
5   MAGENTA
6   BROWN
7   LIGHTGRAY
8   DARKGRAY
9   LIGHTBLUE
10  LIGHTGREEN
11  LIGHTCYAN
12  LIGHTRED
13  LIGHTMAGENTA
14  YELLOW
15  WHITE*/

#include "includeCRI.hpp"

#include "time.h"

using namespace std;

#include "tile.hpp"
#include "map.hpp"

int main() {
	srand(time(0));

	CRI console;
	Map m; 
	vector<int> startCoords = m.initAsBasicCave(200, 200, 10, &console);
	console.setConsoleTitle("Roguedislike");
	console.setCursorVisible(false);
	
	int x = startCoords[0]; 
	int y = startCoords[1];
	bool quit = false;

	while (not quit) {
		if (getKey('W')) {
			if (m.map[y-1][x].walkable) {
				y--;
			}
		}
		if (getKey('S')) {
			if (m.map[y+1][x].walkable) {
				y++;
			}
		}
		if (getKey('A')) {
			if (m.map[y][x-1].walkable) {
				x--;
			}
		}
		if (getKey('D')) {
			if (m.map[y][x+1].walkable) {
				x++;
			}
		}
		if (getKey('Q')) {
			quit = true;
		}
		
		m.renderRaycast(20, 10, x, y, 7, 40);
		console.putChar(20, 10, '@', 0x07);
		sleep(50);
	}
	return 0;
}