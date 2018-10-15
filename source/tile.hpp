#pragma once

class Tile {
	public:
	char tile;
	bool walkable;
	bool transparent;
	bool discovered;
	int colour;
	Tile(char tile, int colour = 0x07, bool walkable = false, bool transparent = false);
	void convertTo(char tile, int colour = 0x07, bool walkable = false, bool transparent = false);
	void drawTile(int atx, int aty);
	void drawHidden(int atx, int aty);
	Tile() = default;
};