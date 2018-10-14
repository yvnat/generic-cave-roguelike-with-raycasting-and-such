#include "map.hpp"
#include "Utils.cpp"
#include "includeCRI.hpp"
#include "tile.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <math.h>

#define RAYSTEP 0.3

void Map::initAsPrototype(int x, int y, CRI * console) {
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
					map[i].push_back(Tile(console, 205, 0x87, false, false));	//||
				}
			} else {
				map[i].push_back(Tile(console, 205, 0x87, false, false));		//=
			}
		}
	}
}

vector<int> Map::initAsBasicCave(int x, int y, int rooms, CRI * console) {
	this->console = console;	//this should be moved to a constructor

	if (x <= 14 || y <= 14) {	//if map is too small, init as prototype instead
		initAsPrototype(x, y, console);
		return {100,100};
	}

	char acceptableGroundTiles[] = {'.','.','.',':'};
	int acceptableGroundColours[] = {0x0e, 0x06};

	//begin by creating a solid stone map
	for (int i = 0; i < y; ++i) {
		map.push_back(vector<Tile>());
		for (int j = 0; j < x; ++j) {
			map[i].push_back(Tile(console, 176, 0x67, false, false));
		}
	}

	vector< vector<int> > roomCoords;	//vector of coordinates
	//then, create several rooms of semirandom sizes in random locations.
	for (int r = 0; r < rooms; ++r) {
		int radius = (rand() % 4) + 2;
		//set a random location and add it to the vector
		int roomX = (rand() % (x - 14)) + 7;
		int roomY = (rand() % (x - 14)) + 7;
		roomCoords.push_back({roomX, roomY});

		//loop through map within radius of room center and turn to ground
		for (int i = roomY - radius; i < roomY + radius; ++i) {
			for (int j = roomX - radius; j < roomX + radius; ++j) {
				char ground = acceptableGroundTiles[rand() % 4];
				int colour = acceptableGroundColours[rand() % 2];
				map[i][j].convertTo(ground, colour, true, true);
			}
		}
	}

	//loop through all rooms and connect them to next room
	for (int i = 0; i < roomCoords.size() - 1; ++i) {
		//a digger is created that goes from the center of one room to the next and digs a path
		int diggerX = roomCoords[i][0];
		int diggerY = roomCoords[i][1];

		int destX = roomCoords[i+1][0];
		int destY = roomCoords[i+1][1];

		while(diggerX != destX || diggerY != destY) {
			//walk
			if (diggerX > destX) {
				diggerX--;
			} else if (diggerX < destX) {
				diggerX++;
			}

			if (diggerY > destY) {
				diggerY--;
			} else if (diggerY < destY) {
				diggerY++;
			}

			//dig
			for (int j = 0; j <= 1; ++j) {
				for (int k = 0; k <= 1; ++k) {
					char ground = acceptableGroundTiles[rand() % 4];
					int colour = acceptableGroundColours[rand() % 2];
					map[diggerY + j][diggerX + k].convertTo(ground, colour, true, true);
				}
			}

		}
	}

	//return coordinates of first room
	return roomCoords[0];
}

RaycastHit* Map::castRay(V<int> start, float theta, float distance) {
	V<float> dir = V<float>((float)cos(theta), (float)sin(theta));
	return castRay(start,dir,distance);
}

RaycastHit* Map::castRay(V<int> start, V<float> dir, float distance){
	V<float> pos = V<float>(start.x,start.y);
	for (float r = 0; r+=RAYSTEP; r<distance) {
		pos = pos+(dir*RAYSTEP);
		int tx = ceil(pos.x);
		int ty = ceil(pos.y);
		if (tx == start.x && ty == start.y) {
			continue;
		}
		Tile thisTile = map[ty][tx];
		if (!thisTile.transparent) {
			RaycastHit rch;
			rch.collided = thisTile;
			rch.distance = r;
			return &rch;
		}
	}
	return nullptr;
}

float Map::castRay(int atx, int aty, int x, int y, float theta, float range) {
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
				if (renderedThisRun.count(to_string(ceil(rayX)) + "," + to_string(ceil(rayY))) == 0) {
					map[ceil(rayY)][ceil(rayX)].drawHidden(atx+(ceil(rayX)-x), aty+(ceil(rayY)-y));
				}
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

void Map::renderRaycast(int atx, int aty, int x, int y, float maxDistance, float numOfRays) {
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

Tile Map::operator()(int x, int y) {
	return map[y][x];
}