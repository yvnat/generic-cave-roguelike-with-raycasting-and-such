#pragma once

#include "includeCRI.hpp"
#include "tile.hpp"

#include <vector>
#include <string>
#include <set>

class Map {
	public:
	CRI * console;
	vector< vector<Tile> > map;
	set<string> renderedThisRun;
	void initAsPrototype(int x, int y, CRI * console);
	vector<int> initAsBasicCave(int x, int y, int rooms, CRI * console);
	float castRay(int atx, int aty, int x, int y, float theta, float range);
	void renderRaycast(int atx, int aty, int x, int y, float maxDistance, float numOfRays);

};