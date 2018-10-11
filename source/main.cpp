/*
The different color codes are

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
15  WHITE
*/
#include "terminalapi/xplatapi.cpp"

#ifdef _WIN32
	#include "winapi.cpp"
#else
	#include "terminalapi/unixapi.cpp"
#endif

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <math.h>
#include "time.h"

using namespace std;

class Tile {
	public:
	char tile;
	bool walkable;
	bool transparent;
	bool discovered;
	int colour;
	CRI * console;
	Tile(CRI * console, char tile, int colour = 0x07, bool walkable = false, bool transparent = false) {
		this->tile = tile;
		this->walkable = walkable;
		this->transparent = transparent;
		this->colour = colour;
		this->console = console;
		discovered = false;
	}
	void drawTile(int atx, int aty) {
		console->putChar(atx, aty, tile, colour);
	}
	void drawHidden(int atx, int aty) {
		if (discovered) {
			console->putChar(atx, aty, tile, 0x01);
		} else {
			console->putChar(atx, aty, ' ');
		}
	}
};

class Map {
	public:
	CRI * console;
	vector< vector<Tile> > map;
	set<string> renderedThisRun;

	void init(int x, int y, CRI * console) {
		this->console = console;
		for (int i = 0; i < y; ++i) {
			map.push_back(vector<Tile>());
			for (int j = 0; j < x; ++j) {
				if ((i != 0) and (i != y-1)) {	//if a middle row
					if ((j != 0) and (j != x-1)) {	//if middle column
						if (rand() % 20 == 1) {
							map[i].push_back(Tile(console, 176, 0x67, false, false));
						} else {
							char toPushBack;	//minimum 32
							if (rand() % 4 == 0) {
								toPushBack = ':';
							} else {
								toPushBack = '.';
							}
							int col;
							if (rand() % 2 == 0) {
								col = 0x0e;	//08 07
							} else {
								col = 0x06;
							}
							map[i].push_back(Tile(console, toPushBack, col, true, true));
						}
					} else {
						map[i].push_back(Tile(console, 186, 0x87));	//||
					}
				} else {
					map[i].push_back(Tile(console, 205, 0x87));		//=
				}
			}
		}
	}
	
	float castRay(int atx, int aty, int x, int y, float theta, float range) {
		float rayX, rayY;
		bool isBlocked = false;
		int lastX, lastY;
		float maxDistance = -1.0;
		for (float rayDistance = 0; rayDistance < range; rayDistance += .3) {
			rayX = (cos(theta) * rayDistance) + x - .5;	//advance by magic of trig
			rayY = (sin(theta) * rayDistance) + y - .5;
			//if it's not the player's tile, draw it			
			if (!((ceil(rayX) == x) && (ceil(rayY) == y))) {
				if (isBlocked && !((ceil(rayY) == lastY) && (ceil(rayX) == lastX))) {	//if the ray has been stopped and this isn't the last known tile
					map[ceil(rayY)][ceil(rayX)].drawHidden(atx+(ceil(rayX)-x), aty+(ceil(rayY)-y));
				} else {
					if (renderedThisRun.count(to_string(ceil(rayX)) + "," + to_string(ceil(rayY))) == 0) {
						renderedThisRun.insert(to_string(ceil(rayX)) + "," + to_string(ceil(rayY)));	//mark as rendered

						map[ceil(rayY)][ceil(rayX)].drawTile(atx+(ceil(rayX)-x), aty+(ceil(rayY)-y));	//draw the tile the ray is at
						//if the tile hasn't been discovered, discover it
						if (!map[ceil(rayY)][ceil(rayX)].discovered) {
							map[ceil(rayY)][ceil(rayX)].discovered = true;
						}
					}
				}
			}
			//if the tile the ray is at is the first non-transparent
			if (!map[ceil(rayY)][ceil(rayX)].transparent && !isBlocked) {
				maxDistance = rayDistance;
				lastX = ceil(rayX);	//note that this is the last visible tile
				lastY = ceil(rayY);
				isBlocked = true;
			}
		}
		if (maxDistance < 0) {
			return range;
		}
		return maxDistance;
	}
	void renderRaycast(int atx, int aty, int x, int y, float maxDistance, float numOfRays) {
		renderedThisRun.clear();

		vector<float> distances;
		for (float r = 0; r < numOfRays; ++r) {
			float theta = 6.28318530718 * (r/numOfRays); //fraction of 2pi
			float dist = castRay(atx, aty, x, y, theta, maxDistance);
			distances.push_back(dist);	//add distance reached to array
		}

		//first person view start
		console->putChar(0, 20, '[', 0x0e);
		for (int i = 0; i < distances.size(); ++i) {	//loop through max distances and render them as FP
			char c = 'E';
			if (distances[i] == 7) {
				c = ' ';
			}
			else if (distances[i] > 4) {
				c = 176;
			}
			else if (distances[i] > 2) {
				c = 177;
			}
			else if (distances[i] > 1) {
				c = 178;
			}
			else {
				c = 219;
			}
			console->putChar(i + 1, 20, c);
		}
		console->putChar(distances.size() + 1, 20, ']', 0x0e);
		//cardinal direction fluff
		//should replace with a view that shows the direction currently looked at
		console->putChar(1, 21, 'E', 0x0c);
		console->putChar((distances.size() * 1 / 4 )+ 1, 21, 'S', 0x0c);
		console->putChar((distances.size() * 2 / 4 )+ 1, 21, 'W', 0x0c);
		console->putChar((distances.size() * 3 / 4 )+ 1, 21, 'N', 0x0c);
		console->putChar((distances.size() * 4 / 4 )+ 1, 21, 'E', 0x0c);
		//first person view end

	}

};

int main() {
	srand(time(0));

	CRI console;
	Map m; m.init(200,200, &console);
	console.setConsoleTitle("Roguedislike");
	console.setCursorVisible(false);
	
	int x = 100; int y = 100;
	bool quit = false;

	while (not quit) {
		if (getKey('W') & 0x8000) {
			if (m.map[y-1][x].walkable) {
				y--;
			}
		}
		if (getKey('S') & 0x8000) {
			if (m.map[y+1][x].walkable) {
				y++;
			}
		}
		if (getKey('A') & 0x8000) {
			if (m.map[y][x-1].walkable) {
				x--;
			}
		}
		if (getKey('D') & 0x8000) {
			if (m.map[y][x+1].walkable) {
				x++;
			}
		}
		if (getKey('Q') & 0x8000) {
			quit = true;
		}
		
		m.renderRaycast(20, 10, x, y, 7, 80);
		console.putChar(20, 10, '@', 0x07);
		sleep(50);
	}
	return 0;
}