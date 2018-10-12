#pragma once

#include "includeCRI.hpp"

class Tile {
	public:
	char tile;
	bool walkable;
	bool transparent;
	bool discovered;
	int colour;
	CRI * console;
	Tile(CRI * console, char tile, int colour = 0x07, bool walkable = false, bool transparent = false);
	void convertTo(char tile, int colour = 0x07, bool walkable = false, bool transparent = false);
	void drawTile(int atx, int aty);
	void drawHidden(int atx, int aty);
};