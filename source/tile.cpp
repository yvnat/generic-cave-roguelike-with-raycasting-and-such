#include "tile.hpp"

Tile::Tile(char tile, int colour, bool walkable, bool transparent) {
	this->tile = tile;
	this->walkable = walkable;
	this->transparent = transparent;
	this->colour = colour;
	discovered = false;
}

void Tile::convertTo(char tile, int colour, bool walkable, bool transparent) {
	this->tile = tile;
	this->walkable = walkable;
	this->transparent = transparent;
	this->colour = colour;
}

void Tile::drawTile(int atx, int aty) {
	// console->putChar(atx, aty, tile, colour);
}

void Tile::drawHidden(int atx, int aty) {
	// if (discovered) {
	// 	console->putChar(atx, aty, tile, 0x01);
	// } else {
	// 	console->putChar(atx, aty, ' ');
	// }
}