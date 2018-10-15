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

#include "Utils.cpp"
#include "tile.hpp"
#include "map.cpp"
#include "Graphics.cpp"
#include "Renderer.cpp"
#include "time.h"

using namespace std;


int main()
{
	srand(time(0));
	Display display(640, 480);

	display.video_init();

	Map m;
	vector<int> startCoords = m.initAsBasicCave(200, 200, 10);
	std::cout << "oofer";

	int x = startCoords[0];
	int y = startCoords[1];
		// if (getKey('W'))
		// {
		// 	if (m.map[y - 1][x].walkable)
		// 	{
		// 		y--;
		// 	}
		// }
		// if (getKey('S'))
		// {
		// 	if (m.map[y + 1][x].walkable)
		// 	{
		// 		y++;
		// 	}
		// }
		// if (getKey('A'))
		// {
		// 	if (m.map[y][x - 1].walkable)
		// 	{
		// 		x--;
		// 	}
		// }
		// if (getKey('D'))
		// {
		// 	if (m.map[y][x + 1].walkable)
		// 	{
		// 		x++;
		// 	}
		// }
		// if (getKey('Q'))
		// {
		// 	quit = true;
		// }

		// m.renderRaycast(20, 10, x, y, 7, 40);
		// console.putChar(20, 10, '@', 0x07);
		// sleep(50);
	bool run = true;
	Renderer rdr(display);

	while (run) {
		for (SDL_Event event : display.get_events()) {
			if (event.type==SDL_QUIT ) {
				run = false;
			}
		}
		rdr.renderMap(m, V<int>(x,y), 20);
		display.refresh();
	}

	return 0;
}