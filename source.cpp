//Asynchronia by yvnat (SpeardwarfErith)

//this file contains all of the code for my project, Asynchronia.
//I admit it may intimidate those who have come unready.
//but ere you split this file apart, I beg you, please consider:
//better giant blob of pasta dough than header hell spaghetti.

#include <iostream>
#include "Windows.h"
#include <string>
#include <array>
#include <vector>
#include "math.h"
#include <unordered_set>
#include <queue>
#include <stack>
#include "time.h"
#include <chrono>
#include <map>
#include <deque>

using namespace std;

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
			toReturn += i.first + ":" + to_string(getAverage(i.second));
		}
		return toReturn;
	}
};
Timer timer;
void setupTimer() {
	timer.createNewSet("fps");
}

class CRI {	//rendering class
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	HANDLE STDHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	vector< vector<string> > screen;
	int cursorX = 0, cursorY = 0;

	public:
	void init(int x, int y, bool unicode) {
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

		/*-----------------Set window to maximized-----------------*/
		ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

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
		string toDisplay = "";
		moveConsoleDrawingPoint(0,0);	//reset the console cursor
		for (int i = 0; i < screen.size(); ++i) {
			for (int j = 0; j < screen[0].size(); ++j) {
				toDisplay += screen[i][j];
			}
			toDisplay += setColour({0,0,0}, {0,0,0}) + "\n";
		}
		DWORD slen=lstrlen(toDisplay.c_str());
		WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE),toDisplay.c_str(),slen,&slen,NULL);
	}

	//this cleans the virtal screen
	void clear() {
		screen = vector<vector<string>>(screen.size(),vector<string>(screen[0].size(), setColour({0,0,0},{0,0,0}) + " "));
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
};

class Log {
	public:
	CRI * console;
	vector<string> messages;
	vector<array<int, 3>> colours;
	array<int, 3> back;
	int x1, y1, x2, y2;	//these are the corners
	void init(CRI * console, int x1, int y1, int x2, int y2, array<int, 3> back = {0, 0, 0}) {
		this->console = console;
		this->back = back;
		if (x1 > x2) {
			this->x1 = x2;
			this->x2 = x1;
		} else {
			this->x1 = x1;
			this->x2 = x2;
		}
		if (y1 > y2) {
			this->y1 = y2;
			this->y2 = y1;
		} else {
			this->y1 = y1;
			this->y2 = y2;
		}
	}
	//this adds a message to the log
	void log(string message, array<int, 3> colour = {255, 255, 255}) {
		messages.push_back(message);
		colours.push_back(colour);
	}
	//this write all messages to screen
	void render() {
		int j = 0;
		//add the messages
		for (int i = messages.size() - 1; i >= 0; --i, ++j) {
			if (y1 + j <= y2) {
				console->putString(x1, y1 + j, messages[i], colours[i], back);
			}
		}
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

class BodyPart {
	public:
	bool disabled, critical, walk, grab;
	string name;
	BodyPart(string name, bool critical, bool walk, bool grab) {
		disabled = false;
		this->name = name;
		this->critical = critical;
		this->walk = walk;
		this->grab = grab;
	}
};
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
	int depth;

	bool acting;
	bool alive;

	Facing dir;
	vector<BodyPart> body;
	bool humanoid;
	int height;	//in cm
	int baseSpeed;

	array<int, 3> fore;
	array<int, 3> bloodColour;

	Entity(char glyph, string name, array<int, 3> fore = {192,192,192}, array<int, 3> bloodColour = {128,0,0}) {
		this->name = name;
		this->glyph = glyph;
		this->fore = fore;
		this->bloodColour = bloodColour;
		dir = N;
		acting = false;
		alive = true;
	}
	void initAsHumanoid() {
		humanoid = true;
		height = 165;
		baseSpeed = 3;
		body.push_back(BodyPart("torso", true, false, false));
		body.push_back(BodyPart("head", true, false, false));
		body.push_back(BodyPart("left arm", false, false, true));
		body.push_back(BodyPart("right arm", false, false, true));
		body.push_back(BodyPart("left leg", false, true, false));
		body.push_back(BodyPart("right leg", false, true, false));
	}
	int getSpeed() {
		int speed = baseSpeed;
		if (humanoid) {
			if (!body[4].disabled && !body[5].disabled) {
				return speed;
			}
			if (!body[4].disabled || !body[5].disabled) {
				return speed * 2;
			}
			return speed * 5;
		} else {
			//when I add non-humanoids this will cause easily identified bugs, as opposed to subtle ones
			return 99999999;
		}
	}
};

///////////////////////////////////////////////   EVENTS   /////////////////////////////////////////////////
 class Event {
	 public:
	 enum EventType {
		 DEFAULT,
		 ENTITYMOVE,
		 size
 	 };
	 int timestamp;
	 EventType type = DEFAULT;
	 Event(int timestamp) {
		 this->timestamp = timestamp;
	 }
 };
 class Event_EntityMove : public Event {
	 public:
	 Entity * subject;
	 int fromx, fromy;
	 int tox, toy;
	 Event_EntityMove(int timestamp, Entity * subject, int fromx, int fromy, int tox, int toy) : Event(timestamp) {
		 type = ENTITYMOVE;
		 this->subject = subject;
		 this->fromx = fromx;
		 this->fromy = fromy;
		 this->tox = tox;
		 this->toy = toy;
	 }
 };

 //ew globals
 vector<Event *> global__eventQ;
 int global__currentTime;
////////////////////////////////////////////////////////////////////////////////////////////////

class Map {
	public:
/*-----------------------Useful Subclasses----------------------*/
	class Tile {
		public:
		char tile;
		array<int, 3> fore, back;
		array<float, 3> staticLight, dynamicLight;
		bool walkable, transparent, discovered;
		bool lightSource;
		array<float, 3> lightEmission;
		string description;
		Tile(char tile, string description = "UNKNOWN TILE", array<int, 3> fore = {192, 192, 192}, array<int, 3> back = {0, 0, 0}, bool walkable = false, bool transparent = false) {
			this->tile = tile;
			this->fore = fore;
			this->back = back;
			this->walkable = walkable;
			this->transparent = transparent;
			staticLight = {.1, .1, .4}; dynamicLight = {0, 0, 0};
			discovered = false;
			lightSource = false;
			lightEmission = {0, 0, 0};
		}
		void initAsLightSource(array<float, 3> light) {
			lightSource = true;
			lightEmission = light;
		}
		array< array<int, 3> , 2> getDisplayColour() {
			array<int, 3> dispf = {static_cast<int>(fore[0] * (staticLight[0] + dynamicLight[0])), static_cast<int>(fore[1] * (staticLight[1] + dynamicLight[1])), static_cast<int>(fore[2] * (staticLight[2] + dynamicLight[2]))};
			array<int, 3> dispb = {static_cast<int>(back[0] * (staticLight[0] + dynamicLight[0])), static_cast<int>(back[1] * (staticLight[1] + dynamicLight[1])), static_cast<int>(back[2] * (staticLight[2] + dynamicLight[2]))};
			array<array<int, 3>, 2> colour = {dispf, dispb};
			return colour;
		}
		array< array<int, 3> , 2> getEntityColour(array<int, 3> ec) {
			array<int, 3> dispf = {static_cast<int>(ec[0] * (staticLight[0] + dynamicLight[0])), static_cast<int>(ec[1] * (staticLight[1] + dynamicLight[1])), static_cast<int>(ec[2] * (staticLight[2] + dynamicLight[2]))};
			array<int, 3> dispb = {static_cast<int>(back[0] * (staticLight[0] + dynamicLight[0])), static_cast<int>(back[1] * (staticLight[1] + dynamicLight[1])), static_cast<int>(back[2] * (staticLight[2] + dynamicLight[2]))};
			array<array<int, 3>, 2> colour = {dispf, dispb};
			return colour;
		}
	};
/*-----------------------general members and constructor----------------------*/
	vector< vector<Tile> > map;
	vector< vector<int> > dijkstraMap;
	vector<Entity> entities;
	CRI * console;

	Map(CRI * console) {
		map = {};
		this->console = console;
	}

/*-----------------------rendering/raycasting--------------------------*/
	//these three sets of coordinates are used in the rendering
	unordered_set<Coord> tilesRevealed;
		//this stores all the tiles revealed by the raycaster
		//they are later rendered.
	unordered_set<Coord> entitiesRendered;
		//this stores the coords of all the entities currently on the screen. 
		//It allows them to be easily accessed for animations and such
	unordered_set<Coord> tilesStaticallyLit;
		//this ensures that tiles will not be lit multiple times by multiple rays from one source
		//it is reset by every light source
	float getLightValue(float light, float distance) {
		return min(light / pow(distance * .5, .6), 1.0);
	}
	void castRay(float fromx, float fromy, float theta, float viewRange, float step, array<float, 3> light, bool lightOnly, bool debug = false) {	//note: upgrade to DDA
		//for debugging
		array<int, 3> rayColour = {rand() % 255, rand() % 255, rand() % 255};	

		float rayX = fromx, rayY = fromy;
		float rayDistance = 0;
	
		//precomputed sin and cos
		float vectorX = cos(theta);
		float vectorY = sin(theta);
	
		while (true) {
			rayDistance += step;
			rayX += vectorX * step;	//advance by magic of trig
			rayY += vectorY * step;

			//prevents out of bound rays, except not really
			if (ceil(rayY) < 0 || ceil(rayX) < 0 || ceil(rayY) >= map.size() || ceil(rayX) >= map[0].size()) {	
				break;
			}
			if (map[ceil(rayY)][ceil(rayX)].transparent && rayDistance < viewRange - 1.5) {	//if this is a valid tile and not blocked before
				if (!lightOnly) {	//'light only' does no rendering, only light
					if (debug) {
						map[ceil(rayY)][ceil(rayX)].back = rayColour;
					}
					map[ceil(rayY)][ceil(rayX)].discovered = true;
					tilesRevealed.insert(Coord(ceil(rayX), ceil(rayY)));
					map[ceil(rayY)][ceil(rayX)].dynamicLight = {getLightValue(light[0], rayDistance), getLightValue(light[1], rayDistance), getLightValue(light[2], rayDistance)};
				} else if (tilesStaticallyLit.count(Coord(ceil(rayX), ceil(rayY))) == 0) {
					//if not already lit by the current light source, light it up
					array<float, 3> tileLight = map[ceil(rayY)][ceil(rayX)].staticLight;
					map[ceil(rayY)][ceil(rayX)].staticLight = {tileLight[0] + getLightValue(light[0], rayDistance), tileLight[1] + getLightValue(light[1], rayDistance), tileLight[2] + getLightValue(light[2], rayDistance)};
					//mark as lit
					tilesStaticallyLit.insert(Coord(ceil(rayX), ceil(rayY)));
				}

			} else {	//if it's not a valid tile (note that this check extends 1 tile out)
				break;
			}
		}
	}

	//this renders stuff and such
	int renderRaycast(int atx, int aty, float fromx, float fromy, float viewRange, float step, bool debug = false, bool coverHidden = true) {
		//clear the rendering sets
		tilesRevealed.clear();
		entitiesRendered.clear();
		//cast the rays
		int numOfRays = 2 * M_PI * viewRange;
		for (int r = 0; r < numOfRays; ++r) {
			float theta = (2 * M_PI) * (static_cast<float>(r) / numOfRays);
			castRay(fromx - .5, fromy - .5, theta, viewRange, step, {1, .8, 0}, false, debug);
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
						map[i.y + iy][i.x + ix].staticLight = map[i.y][i.x].staticLight;
						map[i.y + iy][i.x + ix].dynamicLight = map[i.y][i.x].dynamicLight;
					}
				}
			}
		}

		//adds the new set to the old set
		for (auto i: newWalls) {
			tilesRevealed.insert(i);
		}

		//render the revealed tiles
		for (auto i: tilesRevealed) {
			array<array<int, 3>, 2> tilecolour = map[i.y][i.x].getDisplayColour();
			console->putC(atx + (i.x - fromx), aty + (i.y - fromy), string(1,map[i.y][i.x].tile), tilecolour[0], tilecolour[1]);
		}

		//render entities
		for (int i = 0; i < entities.size(); ++i) {
			if (tilesRevealed.count(entities[i].pos) == 1) {	//if within FOV
				Coord rpos = Coord(atx + (entities[i].pos.x - fromx), aty + (entities[i].pos.y - fromy));
				array<array<int, 3>, 2> rendercolour = map[entities[i].pos.y][entities[i].pos.x].getEntityColour(entities[i].fore);
				console->putC(rpos.x, rpos.y, string(1, entities[i].glyph), rendercolour[0], rendercolour[1]);	//render it
				entitiesRendered.insert(entities[i].pos);
			}
		}

		//return the number of rays used
		return numOfRays;
	}

	void drawMoves(int atx, int aty, int fromx, int fromy, int frame) {
		for(int i = 0; i < global__eventQ.size(); ++i) {	//looping through every event
			//movement events
			if (global__eventQ[i]->type == Event::EventType::ENTITYMOVE) {
				Event_EntityMove * moveEvent = static_cast<Event_EntityMove *>(global__eventQ[i]);
				//don't draw events that ORIGINATE outside FOV
				if (tilesRevealed.count(Coord(moveEvent->fromx, moveEvent->fromy)) != 1) {
					continue;
				}
				string symbol = "E";
				switch (frame) {
					case 0: symbol = string(1, static_cast<char>(250)); break;
					case 1: symbol = string(1, static_cast<char>(249)); break;
					case 2: symbol = string(1, static_cast<char>(254)); break;
					case 3: symbol = string(1, static_cast<char>(219)); break;
					case 4: symbol = string(1, static_cast<char>(219)); break;
					case 5: symbol = string(1, static_cast<char>(219)); break;
				}
				//frame < 6 draw the symbol, frames >= 6 show the ground below
				if (frame < 6) {
					console->putC(atx + (moveEvent->tox - fromx), aty + (moveEvent->toy - fromy), symbol, {0, 255, 0}, {0,0,0});
				} else if (frame == 6) {
					array<array<int, 3>, 2> colour = map[moveEvent->toy][moveEvent->tox].getDisplayColour();
					console->putC(atx + (moveEvent->tox - fromx), aty + (moveEvent->toy - fromy), string(1, map[moveEvent->toy][moveEvent->tox].tile), colour[0], colour[1]);
				}
			}
			//other events, when those will exist
		}
		//this redraws all the entities if the animation is in the first transparent frame
		//note that this is extremely wasteful, but I can't think of a better solution
		if (frame == 6) {
			for (int i = 0; i < entities.size(); ++i) {
				if (entitiesRendered.count(entities[i].pos) == 1) {
					Coord rpos = Coord(atx + (entities[i].pos.x - fromx), aty + (entities[i].pos.y - fromy));
					array<array<int, 3>, 2> rendercolour = map[entities[i].pos.y][entities[i].pos.x].getEntityColour(entities[i].fore);
					console->putC(rpos.x, rpos.y, string(1, entities[i].glyph), rendercolour[0], rendercolour[1]);	//render it
				}
			}
		}
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

	//adds random light sources, for testing purposes
	void addRandomLamps() {
		for (int i = 0; i < 160; ++i) {
			Coord pos = Coord(rand() % map[0].size(), rand() % map.size());
			map[pos.y][pos.x] = Tile('#', "lamp", {255, 255, 255}, {163, 74, 58}, true, true);
			// array<float, 3> colour = {1, .8, 0};
			array<float, 3> colour = {(float)(rand() % 10) / 7, (float)(rand() % 10) / 7, (float)(rand() % 10) / 7};
			map[pos.y][pos.x].initAsLightSource(colour);
		}
	}

	void calculateStaticLight() {
		for (int i = 0; i < map.size(); ++i) {
			for (int j = 0; j < map[0].size(); ++j) {
				if (map[i][j].lightSource) {
					tilesStaticallyLit.clear();
					int viewRange = 30;
					int numOfRays = 2 * M_PI * viewRange;
					for (int r = 0; r < numOfRays; ++r) {
						float theta = (2 * M_PI) * (static_cast<float>(r) / numOfRays);
						castRay(j - .5, i - .5, theta, viewRange, .3, map[i][j].lightEmission, true, false);
					}
				}
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

	bool CA_wallOrFloor_GivenMap(vector< vector<Tile> > map, int x, int y, int sizex, int sizey, int ifWall, int ifEmpty) {	//true = wall, false = floor
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

		// //put the player somewhere
		// player.pos = getValidPos();
	}

	void initAsDungeon_CA(int sizex, int sizey, int numRooms, int roomSizeX, int roomSizeY, int iterations, int initCoverage, int ifWall, int ifEmpty, int aesthetic) {
		//generate the initial map as pure stone
		for (int i = 0; i < sizey; ++i) {
			map.push_back({});
			for (int j = 0; j < sizex; ++j) {
				map[i].push_back(Tile('#'));
			}
		}
		//create an "empty" dijkstra map
		createDijkstraMap();

		//repeat for numRooms times
		for (int r = 0; r < numRooms; ++r) {
			int roomX = rand() % (sizex - roomSizeX - 4) + 2;
			int roomY = rand() % (sizey - roomSizeY - 4) + 2;
			//make a room submap
			vector< vector<Tile> > roomMap;
			
			//initialize it as would to CA cave
			for (int i = 0; i < roomSizeY; ++i) {
				roomMap.push_back({});
				for (int j = 0; j < roomSizeX; ++j) {
					roomMap[i].push_back(Tile('.'));
					if (rand() % 100 <= initCoverage) {
						roomMap[i][j] = (Tile('#'));
					}
				}
			}
			//run CA on THE ONE ROOM
			for (int t = 0; t < iterations; ++t) {
				//this creates a new map and bases it off the old
				//then, it sets that to be the old map
				vector<vector<Tile>> newMap;
				for (int i = 0; i < roomSizeY; ++i) {
					newMap.push_back({});
					for (int j = 0; j < roomSizeX; ++j) {
						//add a floor
						newMap[i].push_back(Tile('.'));
						//if it's a wall
						if (CA_wallOrFloor_GivenMap(roomMap, j, i, roomSizeX, roomSizeY, ifWall, ifEmpty)) {
							newMap[i][j] = (Tile('#'));
						}
					}
				}
				//set the old map to the new map in preparation for next iteration
				roomMap = newMap;
			}

			//combine room with actual map
			for (int i = roomY; i < roomY + roomSizeY; ++i) {
				cout << "\n";
				for (int j = roomX; j < roomX + roomSizeX; ++j) {
					//in case the room ends up being outside map (it shouldn't be)
					if (i >= map.size() || j >= map[0].size()) {
						break;
						cout << "ERROR";
					}
					map[i][j] = roomMap[i - roomY][j - roomX];
					cout << roomMap[i - roomY][j - roomX].tile;
				}
			}
		}

		getCaveAesthetic(aesthetic);
	}

	void getCaveAesthetic(int floorType) {
		/*floortypes:
		 0 sand
		 1 rock
		 2 shrubland
		*/
		// array<int, 3> wcolour = {110 + rand() % 20, 100 + rand() % 20, 93 + rand() % 20};
		array<int, 3> wcolour = {120, 110, 103};
		array<int, 3> fcolour = {static_cast<int>(wcolour[0] / 1.5), static_cast<int>(wcolour[1] / 1.5), static_cast<int>(wcolour[2] / 1.5)};//{75, 70, 80};
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
					const int fv = 8;
					array<int, 3> colour = fcolour;
					colour[0] += (rand() % (fv * 2)) - fv;
					colour[1] += (rand() % (fv * 2)) - fv;
					colour[2] += (rand() % (fv * 2)) - fv;
					map[i][j] = Tile(floorTypes[rand() % floorTypes.size()], floorDesc, colour, {0,0,0}, true, true);
				} else {
					const int cv = 5, dv = 16;
					array<int, 3> colour = wcolour;
					array<int, 3> detailcolour = {60, 66, 66};
					colour[0] += (rand() % (cv * 2)) - cv;
					colour[1] += (rand() % (cv * 2)) - cv;
					colour[2] += (rand() % (cv * 2)) - cv;
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
			if (entities[i].acting) {
				continue;
			}
			if (not entities[i].alive) {
				continue;
			}
			if (rand() % 3 == 0) {
				continue;
			}
			int st[8][2] = {{-1,0},{0,-1},{0,1},{1,0},{-1,-1},{-1,1},{1,-1},{1,1}};
			//loop through surrounding tiles
			for (int j = 0; j < 8; ++j) {
				if (dijkstraMap[entities[i].pos.y+st[j][0]][entities[i].pos.x+st[j][1]] < dijkstraMap[entities[i].pos.y][entities[i].pos.x]
				&& dijkstraMap[entities[i].pos.y+st[j][0]][entities[i].pos.x+st[j][1]] != 0) {
					//entities[i].pos = Coord(entities[i].pos.x+st[j][1], entities[i].pos.y+st[j][0]);
					global__eventQ.push_back(new Event_EntityMove(global__currentTime + entities[i].getSpeed(), &entities[i], entities[i].pos.x, entities[i].pos.y, entities[i].pos.x+st[j][1], entities[i].pos.y+st[j][0]));
					entities[i].acting = true;
					break;
				}
			}
		}
	}

/*-----------------------debugging--------------------------*/
	void _printMap(bool direct = true) {	//prints a full version of the map
		//console->moveConsoleDrawingPoint(1,1);
		cout << console->setColour({192,192,192},{0,0,0});
		for (int i = 0; i < map.size(); ++i) {
			if (not direct) {
				cout << "\n";
			}
			for (int j = 0; j < map[0].size(); ++j) {
				if (direct) {
					array<array<int, 3>, 2> colour = map[i][j].getDisplayColour();
					console->putC(j, i, string(1, map[i][j].tile), colour[0], colour[1]);
				} else {
					array<array<int, 3>, 2> colour = map[i][j].getDisplayColour();
					cout << console->setColour(colour[0], colour[1]);
					cout << map[i][j].tile;
				}
			}
		}
		cout << "\n-done (" << map.size() << ")";
	}
};

class World {
	public:
/*-----------------------general members and constructor-----------------*/
	CRI console;
	Log log;
	vector<Map> dungeon;
	Entity player = Entity('@', "you");

	World(int screenx, int screeny) {
		player.depth = 0;
		player.initAsHumanoid();
		console.init(screenx, screeny, false);
		log.init(&console, 1, 45, 49, 49);
		dungeon.push_back(Map(&console));
		generateLevel(0, 100);
	}
/*-----------------------level gen----------------------*/
	void generateLevel(int type, int numOfGoblins) {
		switch (type) {
			case 0: dungeon[player.depth].initAsCave_CA(200, 200, 10, 45, 4, 5, 1); player.pos = dungeon[player.depth].getValidPos(); break;	//cave
			case 1: dungeon[player.depth].initAsDungeon_CA(100, 100, 5, 20, 20, 5, 45, 4, 5, 1); player.pos = dungeon[player.depth].getValidPos(); break;	//cave
		}
		dungeon[player.depth].addRandomLamps();
		dungeon[player.depth].calculateStaticLight();
		for (int i = 0; i < numOfGoblins; ++i) {
			Entity goblin = Entity('g', "goblin", {108 + rand() % 80, 140 + rand() % 80, 85 + rand() % 80});
			goblin.pos = dungeon[player.depth].getValidPos();
			goblin.initAsHumanoid();
			dungeon[player.depth].entities.push_back(goblin);
		}
	}

/*-----------------------UI----------------------*/
	array<int, 3> getBPCol(BodyPart * bp) {
		if (bp->disabled) {
			return {70, 30, 30};
		}
		return {100, 200, 70};
	}
	void drawBody(int atx, int aty, Entity * entity) {
		if (!entity->humanoid) {
			return;
		}
		array<int, 3> backColour = {0,0,0};
		array<int, 3> torsoColour = getBPCol(&entity->body[0]), headColour = getBPCol(&entity->body[1]);
		array<int, 3> larmColour = getBPCol(&entity->body[2]), rarmColour = getBPCol(&entity->body[3]);
		array<int, 3> llegColour = getBPCol(&entity->body[4]), rlegColour = getBPCol(&entity->body[5]);
		//head
		console.putC(atx + 1, aty, string(1, 222), headColour, backColour);
		console.putC(atx + 2, aty, string(1, 221), headColour, backColour);
		//torso
		console.putC(atx + 1, aty + 1, string(1, 203), torsoColour, backColour);
		console.putC(atx + 2, aty + 1, string(1, 203), torsoColour, backColour);
		// console.putC(atx + 1, aty + 2, string(1, 19), torsoColour, backColour);
		// console.putC(atx + 2, aty + 2, string(1, 19), torsoColour, backColour);
		console.putC(atx + 1, aty + 2, string(1, 199), torsoColour, backColour);
		console.putC(atx + 2, aty + 2, string(1, 182), torsoColour, backColour);
		console.putC(atx + 1, aty + 3, string(1, 198), torsoColour, backColour);
		console.putC(atx + 2, aty + 3, string(1, 181), torsoColour, backColour);
		//arms
		console.putC(atx + 0, aty + 1, string(1, 218), rarmColour, backColour);
		console.putC(atx + 3, aty + 1, string(1, 191), larmColour, backColour);
		console.putC(atx + 0, aty + 2, string(1, 198), rarmColour, backColour);
		console.putC(atx + 3, aty + 2, string(1, 181), larmColour, backColour);
		console.putC(atx + 0, aty + 3, string(1, 222), rarmColour, backColour);
		console.putC(atx + 3, aty + 3, string(1, 221), larmColour, backColour);
		//leg
		console.putC(atx + 1, aty + 4, string(1, 186), rlegColour, backColour);
		console.putC(atx + 2, aty + 4, string(1, 186), llegColour, backColour);
		console.putC(atx + 1, aty + 5, string(1, 179), rlegColour, backColour);
		console.putC(atx + 2, aty + 5, string(1, 179), llegColour, backColour);
	}

/*-----------------------control-----------------------*/
	int renderX = 27, renderY = 23;
	int getCurrentDecaMs() {
		auto now = std::chrono::system_clock::now();
		auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
		auto value = now_ms.time_since_epoch();
		return value.count() % 10000;
	}
	void handleEvents() {
		for (int i = 0; i < global__eventQ.size(); ++i) {
			console.putString(1, 45, to_string(i) + "," + to_string(global__eventQ.size()));
			//if time has not come, continue
			if (global__eventQ[i]->timestamp > global__currentTime) {
				continue;
			}
			//if it is old and unhandled, note error and continue
			if (global__eventQ[i]->timestamp < global__currentTime) {
				continue;
			}

			//go through all the types and handle them
			if (global__eventQ[i]->type == Event::EventType::ENTITYMOVE) {
				Event_EntityMove * moveEvent = static_cast<Event_EntityMove *>(global__eventQ[i]);
				if (moveEvent->subject->alive) {
					moveEvent->subject->pos = Coord(moveEvent->tox, moveEvent->toy);
					moveEvent->subject->acting = false;	//free the entity for new events
				}
			}

			//remove the now-old event and decrements i to account for it
			global__eventQ.erase(global__eventQ.begin() + i--);
		}
	}
	void render() {
		char playerTile = player.glyph;
		console.putC(renderX, renderY, string(1, static_cast<char>(playerTile)), player.fore, {0,0,0});
		dungeon[player.depth].drawMoves(renderX, renderY, player.pos.x, player.pos.y, (getCurrentDecaMs() / 83) % 12);
		drawBody(1, 1, &player);
		log.render();
		console.render();
	}
	void tick(bool debug = false) {
		global__currentTime += 1;
		console.clear();
		dungeon[player.depth].updateDijkstraMap(player.pos.x, player.pos.y, 25, debug);
		handleEvents();
		dungeon[player.depth].advanceEntitiesTowardsPlayer();
		if (not player.acting) {
			dungeon[player.depth].renderRaycast(renderX, renderY, player.pos.x, player.pos.y, 22, .3, false);
		}
	}


/*-----------------------input----------------------*/
	bool getKey(char n) {
    	return GetKeyState(n) & 0x8000;
	}

	//this function handles a request to move the player by a coordinate amount and returns success (update) or failure (no update)
	bool movePlayer(int dX, int dY) {
		int playerX = player.pos.x;
		int playerY = player.pos.y;

		//if player is already doing something, return true
		if (player.acting) {	
			return false;
		}

		//if unwalkable, return false
		if (not dungeon[player.depth].map[playerY + dY][playerX + dX].walkable) {	
			return false;
		}

		//if an entity is there, attack it and return true
		if (dungeon[player.depth].entitiesRendered.count(Coord(playerX + dX, playerY + dY)) > 0) {
			//fight!
			for (int i = 0; i < dungeon[player.depth].entities.size(); ++i) {
				if (dungeon[player.depth].entitiesRendered.count(dungeon[player.depth].entities[i].pos) > 0 && dungeon[player.depth].entities[i].pos == Coord(playerX + dX, playerY + dY)) {
					dungeon[player.depth].map[playerY + dY - 1 + rand() % 3][playerX + dX - 1 + rand() % 3].back = dungeon[player.depth].entities[i].bloodColour;
					int rando = rand() % 6;
					dungeon[player.depth].entities[i].body[rando].disabled = true;
					if (dungeon[player.depth].entities[i].body[0].disabled || dungeon[player.depth].entities[i].body[0].disabled) {
						dungeon[player.depth].entities[i].alive = false;
						dungeon[player.depth].entities[i].glyph = 237;
					}
				}
			}
			//ok done
			return true;
		}

		//move the player, set direction, and return true if made it this far
		//player.pos = Coord(playerX + dX, playerY + dY);
		global__eventQ.push_back(new Event_EntityMove(global__currentTime + player.getSpeed(), &player, playerX, playerY, playerX + dX, playerY + dY));
		player.acting = true;
		if (dX == -1) {
			if (dY == -1) {
				player.dir = Entity::Facing::NW;
			}
			if (dY == 0) {
				player.dir = Entity::Facing::W;
			}
			if (dY == 1) {
				player.dir = Entity::Facing::SW;
			}
		}
		if (dX == 1) {
			if (dY == -1) {
				player.dir = Entity::Facing::NE;
			}
			if (dY == 0) {
				player.dir = Entity::Facing::E;
			}
			if (dY == 1) {
				player.dir = Entity::Facing::SE;
			}
		}
		if (dX == 0) {
			if (dY == -1) {
				player.dir = Entity::Facing::N;
			}
			if (dY == 1) {
				player.dir = Entity::Facing::S;
			}
		}
		return true;
	}
	bool listenPlayerInput() {
		bool update = false;
		if (getKey('E')) {		// top right ↗
			update = movePlayer(1, -1) || update;
		} 
		if (getKey('Q')) {	// top left ↖
			update = movePlayer(-1, -1) || update;
		}
		if (getKey('C')) {	// bottom right ↘
			update = movePlayer(1, 1) || update;
		} 
		if (getKey('Z')) {	// bottom left ↙
			update = movePlayer(-1, 1) || update;
		}
		if (getKey('W')) {	// ↑
			update = movePlayer(0, -1) || update;
		}
		if (getKey('S')) {	// ↓
			update = movePlayer(0, 1) || update;
		}
		if (getKey('A')) {	// ←
			update = movePlayer(-1, 0) || update;
		}
		if (getKey('D')) {	// →
			update =  movePlayer(1, 0) || update;
		}
		if (getKey('X')) {	// stay in place
			update = true;
			log.log("imma wait mk");
			Sleep(50);
		}
		if (getKey('R')) {	// random injury
			player.body[rand() % player.body.size()].disabled = true;
			Sleep(50);
		}

		return update;
	}
};

int main(int argc, char* argv[]) {
	srand(time(0));

	World w = World(50, 60);

	bool quit = false;
	bool change = true;

	setupTimer();

	cout << "STARTING";
	while (not quit) {
		if (GetKeyState(VK_ESCAPE) & 0x8000) {
			quit = true;
		}
		if (change) {
			w.tick();
			change = false;
		}
		if (w.player.acting) {
			change = true;
		} else {
			w.render();
			change = w.listenPlayerInput();
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
