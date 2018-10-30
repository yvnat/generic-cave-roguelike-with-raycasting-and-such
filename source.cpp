//Asynchronia by yvnat (SpeardwarfErith)

//this file contains all of the code for my project, Asynchronia.
//I admit it may intimidate those who have come unready.
//but ere you split this file apart, I beg you, please consider:
//better giant blob of pasta dough than header hell spaghetti.

//these are for CRI
#include <iostream>
#include "Windows.h"
#include <string>
#include <array>

//these are for map
#include <vector>
//#include <set>
#include "math.h"
#include <unordered_set>
//#include <array> already included in CRI
//#include <string> already included in CRI
#include <queue>

//these are for entity
//#include <array> already included in CRI
//#include <string> already included in CRI
#include <stack>

//this is only for rand()
#include "time.h"

//for debugging purposes
#include <chrono>
#include <map>

using namespace std;

class CRI {	//rendering class
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	HANDLE STDHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	vector< vector<string> > screen;
	int cursorX = 0, cursorY = 0;

	public:
	void init(int x, int y, bool unicode) {
		/*stuff that makes sense*/
		for (int i = 0; i < y; ++i) {
			screen.push_back({});
			for (int j = 0; j < x; ++j) {
				screen[i].push_back(setColour({0,0,0}, {0,0,0}) + " ");
			}
		}

		#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
		#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
		#endif

		/*-----------Enable Unicode-----------*/
		//note that a valid font is still needed
		if (unicode) {
			SetConsoleCP(65001);  // 65001 = UTF-8
			SetConsoleOutputCP(65001);
		}

		/*---------------Enable ANSI--------------*/
		// Set output mode to handle virtual terminal sequences
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hOut == INVALID_HANDLE_VALUE) {
			cout << GetLastError();
		}

		DWORD dwMode = 0;
		if (!GetConsoleMode(hOut, &dwMode)) {
			cout << GetLastError();
		}

		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		if (!SetConsoleMode(hOut, dwMode)){
			cout << GetLastError();
		}

		/*---------------Hide Cursor--------------*/
		CONSOLE_CURSOR_INFO cursorInfo;
		GetConsoleCursorInfo(STDHandle, &cursorInfo);
		cursorInfo.bVisible = false; // set the cursor visibility
		SetConsoleCursorInfo(STDHandle, &cursorInfo);
	}

	/*------------------------------CONSOLE SETTINGS-----------------------------------*/
	void setConsoleTitle(string title) {
		SetConsoleTitle(title.c_str());
	}

	/*------------------------------OUTPUT-----------------------------------*/
	//this moves the actual console's cursor.
	void moveConsoleDrawingPoint(int x, int y) {
		SetConsoleCursorPosition(STDHandle, {static_cast<SHORT>(x), static_cast<SHORT>(y)});
	}

	//this moves the virtual cursor on the screen grid
	void moveCursor(int x, int y) {
		cursorX = x; cursorY = y;
	}

	//this writes a string to a single square of the screen grid. Recommended use is a single char + ANSI colour
	void print(const string input) {
		if (cursorX < screen[0].size() && cursorY < screen.size()) {	//if within the screen
			screen[cursorY][cursorX] = input;
		}
	}

	//this writes a string char by char to successive tiles. Used to properly show text.
	void printString(const string input) {
		for (int i = 0; i < input.length(); ++i) {
			if (cursorX + i < screen[0].size() && cursorY < screen.size()) {	//if within the screen
				screen[cursorY][cursorX + i] = input[i];
			}
		}
	}

	//returns an ANSI escape code for the given RGB value
	string setColour(array<int, 3> fore, array<int, 3> back) {
		return "\033[38;2;" + to_string(fore[0]) + ";" + to_string(fore[1]) + ";" + to_string(fore[2]) + "m"	//foreground
			 + "\033[48;2;" + to_string(back[0]) + ";" + to_string(back[1]) + ";" + to_string(back[2]) + "m";	//background

	}

	//put an input string (recommended ANSI colour + single char) to a SINGLE TILE
	void put(const int x, const int y, const string c) {
		moveCursor(x, y);
		print(c);
	}

	//same as put(), but with colour
	void putC(int x, int y, string c, array<int, 3> fore, array<int, 3> back) {
		moveCursor(x, y);
		print(setColour(fore, back) + c);
	}

	//put an input string to MULTIPLE ADJACENT TILES
	void putString(int x, int y, string c, array<int, 3> fore = {192, 192, 192}, array<int, 3> back = {0,0,0}) {
		moveCursor(x, y);
		printString(setColour(fore, back) + c);
	}

	//this assembles screen into a string, then outputs it to the console
	void render() {	
		moveConsoleDrawingPoint(0,0);	//reset the console cursor
		string toDisplay = "";
		for (int i = 0; i < screen.size(); ++i) {
			for (int j = 0; j < screen.size(); ++j) {
				toDisplay += screen[i][j];
			}
			toDisplay += setColour({0,0,0}, {0,0,0}) + "\n";
		}
		DWORD slen=lstrlen(toDisplay.c_str());
		//probably faster than cout
		WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE),toDisplay.c_str(),slen,&slen,NULL);
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
};

class Timer { //debugging use
	public:
	map<string, vector<float> > dataSets;
	std::chrono::_V2::system_clock::time_point start = chrono::high_resolution_clock::now();    

	Timer() {
		cout << "yay";
	}

	void createNewSet(string name) {
		dataSets[name] = {};
	}
	void startTimer() {
		start = chrono::high_resolution_clock::now();
	}
	void endTimer(string set) {
		std::chrono::_V2::system_clock::time_point end = chrono::high_resolution_clock::now();
		dataSets[set].push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
	}

	float getAverage(vector<float> data) {
		float total = 0;
		for (int i = 0; i < data.size(); ++i) {
			total += data[i];
		}
		return total / data.size();
	}
	string returnData() {
		string toReturn = "";
		for (auto i: dataSets) {
			toReturn += i.first + ":" + to_string(getAverage(i.second)) + "\n";
		}
		return toReturn;
	}
};

struct Coord {
	public:
	int x, y;
	Coord(int x, int y) {
		this->x = x;
		this->y = y;
	}
	bool operator< (const Coord& coord) const {
		return (x < coord.x) || (x == coord.x && y < coord.y);
	}
	bool operator== (const Coord& coord) const {
		return (x == coord.x) && (y == coord.y);
	}
};
namespace std {	//this implements hashing of Coord, somehow.
	template <>
	struct hash<Coord> {
		std::size_t operator()(const Coord& c) const {
			using std::size_t;
			using std::hash;
			return ((hash<int>()(c.x)
					^ (hash<int>()(c.y) << 1)) >> 1);
		}
	};
}

//////////////////////////////////////////////////////////////HERE BE EVENTS
 //events are stored in the Timeline stack in World
 //they represent events
 enum EventType {
	 ENTITY_MOVE,
	 ENTITY_ROTATE,
 };
 class Event {
	 public:
	 int timestamp;
	 EventType id;
	 Event(int timestamp, EventType id) {
		 this->timestamp = timestamp;
		 this->id = id;
	 }
 };
 class Event_EntityMove : public Event {
	 public:
	 int tox, toy;
	 int fromx, fromy;
	 Event_EntityMove(int timestamp, int tox, int toy, int fromx, int fromy) : Event(timestamp, EventType::ENTITY_MOVE) {
		 this->tox = tox;
		 this->toy = toy;
		 this->fromx = fromx;
		 this->fromy = fromy;
	 }
 };
//////////////////////////////////////////////////////////////

class Entity {
	public:
/*-------------------general members and constructor----------------------*/
	//each facing has the corresponding cp437 value attached to it
	enum Facing {
		N = 94,
		S = 118,
		E = 62,
		W = 60,
		NE = 191,
		NW = 218,
		SE = 217,
		SW = 192,
	};

	char glyph;
	string name;
	Coord pos = Coord(-1,-1);
	Facing dir;

	array<int, 3> fore;
	array<int, 3> bloodColour;

	Entity(char glyph, string name, array<int, 3> fore = {192,192,192}, array<int, 3> bloodColour = {128,0,0}) {
		this->name = name;
		this->glyph = glyph;
		this->fore = fore;
		this->bloodColour = bloodColour;
		dir = N;
	}

};

class Player: public Entity {
	public:
	Player() : Entity('@', "you", {192,192,192}) {}

};

class Map {
	public:
/*-------------------Useful Subclasses----------------------*/
	class Tile {
		public:
		char tile;
		array<int, 3> fore, back;
		bool walkable, transparent, discovered;
		string description;
		Tile(char tile, string description = "UNKNOWN TILE", array<int, 3> fore = {192, 192, 192}, array<int, 3> back = {0, 0, 0}, bool walkable = false, bool transparent = false) {
			this->tile = tile;
			this->fore = fore;
			this->back = back;
			this->walkable = walkable;
			this->transparent = transparent;
			discovered = false;
		}
	};
/*-------------------general members and constructor----------------------*/
	vector< vector<Tile> > map;
	vector< vector<int> > dijkstraMap;
	vector<Entity> entities;
	Entity player = Player();
	CRI * console;

	Map(CRI * console) {
		map = {};
		this->console = console;
	}

/*-----------------------map generation--------------------------*/
	//this creates an "empty" dijkstra map based on map size
	void createDijkstraMap() {
		for (int i = 0; i < map.size(); ++i) {
			dijkstraMap.push_back({});
			for (int j = 0; j < map[0].size(); ++j) {
				dijkstraMap[i].push_back(999999);
			}
		}
	}

	bool CA_wallOrFloor(int x, int y, int sizex, int sizey, int ifWall, int ifEmpty) {	//true = wall, false = floor
		int countW = 0;
		//this counts how many walls are surrounding it
		for (int i = -1; i <= 1; ++i) {
			for (int j = -1; j <= 1; ++j) {
				bool outOfBound = false;
				//these watch for out of bounds tiles
				//
				//note: out of bounds tiles are treated as walls for counting purposes
				//
				if ((y+i < 0) || (y+i >= sizey)) {	//if y out of range
					outOfBound = true;
					countW++;
				}
				if ((x+j < 0) || (x+j >= sizex)) {  //if x out of range
					outOfBound = true;
					countW++;
				}
				if (i == 0 && j == 0) {	//not strictly necessary, but ignores the actual tile itself
					outOfBound = true;
				}
				//if hasn't failed yet, counts the tile
				if (not outOfBound) {
					if (map[y+i][x+j].tile == '#') {
						countW++;
					}
				}
			}
		}
		//if more than ifEmpty of its surroundings are walls, 
		//or it is already a wall and ifWall of its surroundings are walls, it will be a wall
		//else, it will be a floor
		if ((map[y][x].tile == '.') && (countW >= ifEmpty)) {
			return true;
		} else if ((map[y][x].tile == '#') && (countW >= ifWall)) {
			return true;
		} else {
			return false;
		}
	}
	/*
	 *this creates a cavelike map using cellular automata
	 *the initCoverage determines what % of the map is initially walls
	 *ifWall and ifEmpty determine the number of surrounding walls needed
	 to turn the tile into a wall if it is already a wall or if it is empty
	*/
	void initAsCave_CA(int sizex, int sizey, int iterations, int initCoverage, int ifWall, int ifEmpty, int aesthetic, bool debugPrint = false) {
		//generate the initial map
		for (int i = 0; i < sizey; ++i) {
			map.push_back({});
			for (int j = 0; j < sizex; ++j) {
				if (((i <= 2) || (i >= sizey - 3)) || ((j <= 2) || (j >= sizex - 3))) {	//if edge, make it wall
					map[i].push_back(Tile('#'));
				} else {
					//add a floor
					map[i].push_back(Tile('.'));
					//45% chance of actually making it a wall
					if (rand() % 100 <= initCoverage) {
						map[i][j] = (Tile('#'));
					}
				}			
			}
		}
		//create an "empty" dijkstra map
		createDijkstraMap();

		//smooth the map
		for (int t = 0; t < iterations; ++t) {
			if (debugPrint) {
				cout << "\n\ngeneration " << t;
				_printMap();
			}
			//this creates a new map and bases it off the old
			//then, it sets that to be the old map
			vector<vector<Tile>> newMap;
			for (int i = 0; i < sizey; ++i) {
				newMap.push_back({});
				for (int j = 0; j < sizex; ++j) {
					//add a floor
					newMap[i].push_back(Tile('.'));
					//if it's a wall
					if (CA_wallOrFloor(j, i, sizex, sizey, ifWall, ifEmpty)) {
						newMap[i][j] = (Tile('#'));
					}
				}
			}
			//set the old map to the new map in preparation for next iteration
			map = newMap;
		}

		//post processing to make it a valid and pretty map
		getCaveAesthetic(aesthetic);

		//put the player somewhere
		player.pos = getValidPos();
	}

	void getCaveAesthetic(int floorType) {
		/*floortypes:
		 0 sand
		 1 rock
		 2 shrubland
		*/
		array<int, 3> fcolour = {128, 128, 96};
		array<int, 3> wcolour = {50, 40, 50};
		vector<char> floorTypes;
		string floorDesc;
		switch (floorType) {
			case 0: floorTypes = {static_cast<char>(247), '~'}; floorDesc = "sand"; break;
			case 1: floorTypes = {'.',':',static_cast<char>(249)}; floorDesc = "rock floor"; break;
			case 2: floorTypes = {'.',',','\'',':','#'}; floorDesc = "shrubland"; fcolour = {128, 128, 96}; break;
			case 3: floorTypes = {'.',',','\'',':','"','#'}; floorDesc = "grass"; fcolour = {167, 200, 124}; break;
			default: floorTypes = {'E'};
		}


		for (int i = 0; i < map.size(); ++i) {
			for (int j = 0; j < map[0].size(); ++j) {
				if (map[i][j].tile == '.') {
					array<int, 3> colour = fcolour;
					colour[0] += (rand() % 32) - 16;
					colour[1] += (rand() % 32) - 16;
					colour[2] += (rand() % 32) - 16;
					map[i][j] = Tile(floorTypes[rand() % floorTypes.size()], floorDesc, colour, {0,0,0}, true, true);
				} else {
					const int cv = 5, dv = 16;
					array<int, 3> colour = wcolour;
					colour[0] += (rand() % (cv * 2)) - cv;
					colour[1] += (rand() % (cv * 2)) - cv;
					colour[2] += (rand() % (cv * 2)) - cv;
					array<int, 3> detailcolour = {90, 100, 100};
					detailcolour[0] += (rand() % (dv * 2)) - dv;
					detailcolour[1] += (rand() % (dv * 2)) - dv;
					detailcolour[2] += (rand() % (dv * 2)) - dv;
					map[i][j] = Tile(176, "cavern wall", detailcolour, colour);
				}
			}
		}
	}

	Coord getValidPos() {
		bool properlyPlaced = false;
		Coord pos = Coord(-1, -1);
		//randomize until coordinate is a floor
		while (not properlyPlaced) {
			pos = Coord(rand() % map[0].size(), rand() % map.size());
			properlyPlaced = map[pos.y][pos.x].walkable;
		}
		//set position and add
		return pos;
	}


/*-----------------------raycasting--------------------------*/
	//these three sets of coordinates are used in the rendering
	unordered_set<Coord> tilesRevealed;
		//this stores all the tiles revealed by the raycaster
		//they are later rendered.
	unordered_set<Coord> tilesToHide;
		//this stores all the tiles within range but NOT revealed.
		//they are used to cover up the previously rendered tiles instead of clearing the screen
	unordered_set<Coord> entitiesRendered;
		//this stores all the entities currently on the screen. 
		//It prevents them from flashing due to being overwritten by a floor when moving
	void castRay(float fromx, float fromy, float theta, float viewRange, float step, bool renderHidden = true, bool debug = false) {	//note: upgrade to DDA
		//for debugging
		array<int, 3> rayColour = {rand() % 255, rand() % 255, rand() % 255};	

		float rayX = fromx, rayY = fromy;
		float rayDistance = 0;
	
		//precomputed sin and cos
		float vectorX = cos(theta);
		float vectorY = sin(theta);
	
		bool blocked = false;
		while (true) {
			rayDistance += step;
			if (blocked) {	//goes 3 times as fast if blocked already
				rayDistance += 2 * step;
				rayX += 2 * vectorX * step;
				rayY += 2 * vectorY * step;
			}
			rayX += vectorX * step;	//advance by magic of trig
			rayY += vectorY * step;

			//prevents out of bound rays
			if (ceil(rayY) < 0 || ceil(rayX) < 0 || ceil(rayY) >= map.size() || ceil(rayX) >= map[0].size()) {	
				break;
			}
			if (map[ceil(rayY)][ceil(rayX)].transparent && !blocked && rayDistance < viewRange - 1.5) {	//if this is a valid tile and not blocked before
				if (debug) {
					map[ceil(rayY)][ceil(rayX)].back = rayColour;
				}
				map[ceil(rayY)][ceil(rayX)].discovered = true;
				tilesRevealed.insert(Coord(ceil(rayX), ceil(rayY)));
			} else {	//if it's not a valid tile (note that this check extends 1 tile out)
				blocked = true;
				if (debug) {
					tilesRevealed.insert(Coord(ceil(rayX), ceil(rayY)));
					// tilesToHide.insert(Coord(ceil(rayX), ceil(rayY)));	//add it to the tiles to hide
				} else if (renderHidden) {
					tilesToHide.insert(Coord(ceil(rayX), ceil(rayY)));	//add it to the tiles to hide
					// tilesRevealed.insert(Coord(ceil(rayX), ceil(rayY)));
				} else {
					break;	//or break, if that setting is disable
				}
			}
			if (rayDistance > viewRange) {
				break;
			}
		}
	}
	/*
	 *atx/y- where on the screen the rendering should be centered
	 *fromx/y- where on the map the raycasting should be centered
	 *numOfRays- number of rays to cast
	 *viewRange- max distance ray travels
	 *returns number of rays casted
	*/
	void renderEntities(int atx, int aty, float fromx, float fromy, bool arrows, bool all = false) {
		for (int i = 0; i < entities.size(); ++i) {
			if (tilesRevealed.count(entities[i].pos) == 1) {	//if within FOV
				Coord rpos = Coord(atx + (entities[i].pos.x - fromx), aty + (entities[i].pos.y - fromy));

				char symbolToRender = entities[i].glyph;
				if (arrows && (((abs(entities[i].pos.x - player.pos.x) <= 4) && (abs(entities[i].pos.y - player.pos.y) <= 4)) || all)) {
					symbolToRender = entities[i].dir;
				}

				console->putC(rpos.x, rpos.y, string(1, symbolToRender), entities[i].fore, {0,0,0});	//render it
				entitiesRendered.insert(entities[i].pos);
			}
		}
	}
	int renderRaycast(int atx, int aty, float fromx, float fromy, float viewRange, float step, bool debug = false, bool coverHidden = true) {
		//clear the rendering sets
		tilesRevealed.clear();
		tilesToHide.clear();
		entitiesRendered.clear();

		//cast the rays
		int numOfRays = 2 * M_PI * viewRange;
		for (int r = 0; r < numOfRays; ++r) {
			float theta = (2 * M_PI) * (static_cast<float>(r) / numOfRays);
			castRay(fromx - .5, fromy - .5, theta, viewRange, step, true, debug);
		}
		//post processing to show walls that are adjacent to revealed floors
		unordered_set<Coord> newWalls;
		//goes through every revealed tile and adds all adjacent walls to new set
		for (auto i: tilesRevealed) {
			for (int iy = -1; iy <= 1; ++iy) {
				for (int ix = -1; ix <= 1; ++ix) {
					if (map[i.y + iy][i.x + ix].transparent == false) {
						newWalls.insert(Coord(i.x + ix, i.y + iy));
						map[i.y + iy][i.x + ix].discovered = true;
					}
				}
			}
		}
		//adds the new set to the old set
		for (auto i: newWalls) {
			tilesRevealed.insert(i);
		}

		//render entities
		//renderEntities(atx, aty, fromx, fromy);
		// for (int i = 0; i < entities.size(); ++i) {
		// 	if (tilesRevealed.count(entities[i].pos) == 1) {	//if within FOV
		// 		Coord rpos = Coord(atx + (entities[i].pos.x - fromx), aty + (entities[i].pos.y - fromy));

		// 		char symbolToRender = entities[i].glyph;
		// 		if (time(0) % 2 == 0 && (abs(entities[i].pos.x - player.pos.x) <= 4) && (abs(entities[i].pos.y - player.pos.y) <= 4)) {
		// 			symbolToRender = entities[i].dir;
		// 		}

		// 		console->putC(rpos.x, rpos.y, string(1, symbolToRender), entities[i].fore, {0,0,0});	//render it
		// 		entitiesRendered.insert(entities[i].pos);
		// 	}
		// }

		//render the revealed tiles
		for (auto i: tilesRevealed) {
			if ((i.x != fromx || i.y != fromy) && (entitiesRendered.count(i) == 0)) {	//if not center tile and doesn't have an entity
				console->putC(atx + (i.x - fromx), aty + (i.y - fromy), string(1,map[i.y][i.x].tile), map[i.y][i.x].fore, map[i.y][i.x].back);
			}
		}
		//render the unrevealed tiles
		if (coverHidden) {
			for (auto i: tilesToHide) {
				if (tilesRevealed.count(i) == 0) {	//this stops it from blacking over walls that are added in postproc
					console->put(atx + (i.x - fromx), aty + (i.y - fromy), console->setColour({0,0,0}, {0,0,0}) + " ");
				}
			}
		}
		
		//render player!
		console->putC(atx, aty, string(1, player.glyph), player.fore, {0,0,0});

		
		//return the number of rays used
		return numOfRays;
	}

/*-----------------------dijkstra map/pathfinding--------------------------*/
	void updateDijkstraMap(int playerx, int playery, int range, bool debug = false) {
		//each queue in the vector represents a "layer" of pathfinding
		vector< queue<Coord> > frontier;
		unordered_set<Coord> visited;
		//create frontier[0]
		frontier.push_back(queue<Coord>());
		//add the player's location to it
		frontier[0].push(Coord(playerx, playery));

		//loop through <range> layers
		for (int i = 0; i < range; ++i) {
			//create the next frontier
			frontier.push_back(queue<Coord>());
			while (not frontier[i].empty()) {
				Coord currentTile = frontier[i].front(); frontier[i].pop();	//why does pop not return value? fuckin c++
				//check if already visited, skip if yes
				if (visited.count(currentTile) > 0) {
					continue;
				}
				visited.insert(currentTile);
				//set the value to be the current layer
				dijkstraMap[currentTile.y][currentTile.x] = i;
				
				//surrounding tiles
				int st[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
				//loop through surrounding tiles
				for (int j = 0; j < 8; ++j) {
					Coord surroundingTile = Coord(currentTile.x + st[j][1],currentTile.y + st[j][0]);
					//check if they are passable
					if (map[surroundingTile.y][surroundingTile.x].walkable) {
						//if they are, add them to the next frontier
						frontier[i+1].push(surroundingTile);
					}
				}
			}
		}
		//if debug mode is on, it colours the tiles based on their dijkstra
		if (debug) {
			for (int i = 0; i < dijkstraMap.size(); ++i) {
				for (int j = 0; j < dijkstraMap[0].size(); ++j) {
					int layer = dijkstraMap[i][j];
					if (layer > 25) {
						layer = 25;
					}
					map[i][j].back = {layer * 10, layer * 10, layer * 10};
				}	
			}
		}
	}

	void advanceEntitiesTowardsPlayer() {
		for (int i = 0; i < entities.size(); ++i) {
			if (rand() % 3 == 0) {
				continue;
			}
			int st[8][2] = {{-1,0},{0,-1},{0,1},{1,0},{-1,-1},{-1,1},{1,-1},{1,1}};
			//loop through surrounding tiles
			for (int j = 0; j < 8; ++j) {
				if (dijkstraMap[entities[i].pos.y+st[j][0]][entities[i].pos.x+st[j][1]] < dijkstraMap[entities[i].pos.y][entities[i].pos.x]
				&& dijkstraMap[entities[i].pos.y+st[j][0]][entities[i].pos.x+st[j][1]] != 0) {
					entities[i].pos = Coord(entities[i].pos.x+st[j][1], entities[i].pos.y+st[j][0]);
					switch(j) {
						case 0:entities[i].dir = Entity::N; break;
						case 1:entities[i].dir = Entity::W; break;
						case 2:entities[i].dir = Entity::E; break;
						case 3:entities[i].dir = Entity::S; break;
						case 4:entities[i].dir = Entity::NW; break;
						case 5:entities[i].dir = Entity::NE; break;
						case 6:entities[i].dir = Entity::SW; break;
						case 7:entities[i].dir = Entity::SE; break;
					}
					break;
				}
			}
		}
	}
/*-----------------------debugging--------------------------*/
	void _printMap(bool direct = true) {	//prints a full version of the map
		cout << console->setColour({192,192,192},{0,0,0});
		for (int i = 0; i < map.size(); ++i) {
			if (not direct) {
				cout << "\n";
			}
			for (int j = 0; j < map[0].size(); ++j) {
				if (direct) {
					console->putC(j, i, string(1, map[i][j].tile), map[i][j].fore, map[i][j].back);
				} else {
					cout << console->setColour(map[i][j].fore, map[i][j].back);
					cout << map[i][j].tile;
				}
			}
		}
		// for (int i = 0; i < entities.size(); ++i) {
		// 	console->putC(entities[i].pos.x, entities[i].pos.y, string(1, entities[i].glyph), entities[i].fore, {0,0,0});
		// 	//cout << entities[i].pos.x << " " << entities[i].pos.y << endl;
		// }
		
	}
};

class World {
	public:
/*-------------------general members and constructor----------------------*/
	CRI console;
	vector<Map> dungeon;
	int currentLevel;
	int time;
	stack<Event> timeline;

	World(int screenx, int screeny) {
		console.init(screenx, screeny, false);
		dungeon.push_back(Map(&console));
		currentLevel = 0;
		generateLevel(0, 50);
	}
/*----------------------level gen----------------------*/
	void generateLevel(int type, int numOfGoblins) {
		switch (type) {
			case 0: dungeon[currentLevel].initAsCave_CA(100, 100, 3, 45, 4, 5, 1); break;	//cave
		}
		for (int i = 0; i < numOfGoblins; ++i) {
			Entity goblin = Entity('g', "goblin", {108 + rand() % 80, 140 + rand() % 80, 85 + rand() % 80});
			goblin.pos = dungeon[currentLevel].getValidPos();
			dungeon[currentLevel].entities.push_back(goblin);
		}
	}

/*----------------------control- put loop and tick() and such here----------------------*/


/*----------------------input----------------------*/
	bool getKey(char n) {
    	return GetKeyState(n) & 0x8000;
	}

	//this function handles a request to move the player by a coordinate amount and returns success (update) or failure (no update)
	bool movePlayer(int dX, int dY) {
		int playerX = dungeon[currentLevel].player.pos.x;
		int playerY = dungeon[currentLevel].player.pos.y;

		//if unwalkable, return false
		if (not dungeon[currentLevel].map[playerY + dY][playerX + dX].walkable) {	
			return false;
		}

		//if an entity is there, attack it and return true
		if (dungeon[currentLevel].entitiesRendered.count(Coord(playerX + dX, playerY + dY)) > 0) {
			//fight!
			return true;
		}

		//move the player and return true
		dungeon[currentLevel].player.pos = Coord(playerX + dX, playerY + dY);

		//return true if made it this far
		return true;
	}
	bool listenPlayerInput() {
		bool update = false;
		if (getKey('U')) {		// top right ↗
			update = movePlayer(1, -1) || update;
		} 
		if (getKey('Y')) {	// top left ↖
			update = movePlayer(-1, -1) || update;
		}
		if (getKey('N')) {	// bottom right ↘
			update = movePlayer(1, 1) || update;
		} 
		if (getKey('B')) {	// bottom left ↙
			update = movePlayer(-1, 1) || update;
		}
		if (getKey('K')) {	// ↑
			update = movePlayer(0, -1) || update;
		}
		if (getKey('J')) {	// ↓
			update = movePlayer(0, 1) || update;
		}
		if (getKey('H')) {	// ←
			update = movePlayer(-1, 0) || update;
		}
		if (getKey('L')) {	// →
			update =  movePlayer(1, 0) || update;
		}
		if (getKey('X')) {	// stay in place
			update = true;
		}

		return update;
	}
};

int main(int argc, char* argv[]) {
	srand(time(0));

	World w = World(45, 45);

	bool quit = false;
	bool change = true;

	bool arrowState = false;
	bool renderedEvenTime = false;
	bool renderedOddTime = false;

	cout << "STARTING";
	while (not quit) {
		if (GetKeyState(VK_ESCAPE) & 0x8000) {
			quit = true;
		}
		change = w.listenPlayerInput();
		if (time(0) % 2 == 0) {
			renderedOddTime = false;
			if (!renderedEvenTime) {
				w.dungeon[w.currentLevel].renderEntities(23, 23, w.dungeon[w.currentLevel].player.pos.x, w.dungeon[w.currentLevel].player.pos.y, arrowState);
				w.console.render();
				arrowState = !arrowState;
				renderedEvenTime = true;
			}
		} else {
			renderedEvenTime = false;
			if (!renderedOddTime) {
				w.dungeon[w.currentLevel].renderEntities(23, 23, w.dungeon[w.currentLevel].player.pos.x, w.dungeon[w.currentLevel].player.pos.y, arrowState);
				w.console.render();
				arrowState = !arrowState;
				renderedOddTime = true;
			}
		}
		if (change) {
			renderedOddTime = false; renderedEvenTime = false;
			w.dungeon[w.currentLevel].updateDijkstraMap(w.dungeon[w.currentLevel].player.pos.x, w.dungeon[w.currentLevel].player.pos.y, 25, false);
			w.dungeon[w.currentLevel].advanceEntitiesTowardsPlayer();
			w.dungeon[w.currentLevel].renderEntities(23, 23, w.dungeon[w.currentLevel].player.pos.x, w.dungeon[w.currentLevel].player.pos.y, arrowState);
			w.dungeon[w.currentLevel].renderRaycast(23, 23, w.dungeon[w.currentLevel].player.pos.x, w.dungeon[w.currentLevel].player.pos.y, 22, .1, false);
			w.console.render();
			change = false;
		    cout << w.console.setColour({0,0,0},{255,255,255}) << " " << w.console.setColour({255,255,255},{0,0,0});
			cout << "\n < " + string(1, (char)218) + " ^ " + string(1, (char)191) + " > " + string(1,(char)217) + " v ";
			Sleep(50);
		}
	}
	cout << "SUCCESSFULLY FINISHED RUNNING";
	return 0;
}



/*

    ↓ → →
    ↓ @ ↑
    → → →

*/
