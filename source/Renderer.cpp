#include "Graphics.cpp"
#include "Utils.cpp"
#include "map.cpp"

class Renderer {
    public:
    Display display;
    Renderer(Display d) {
        this->display = d;
    }
    void renderMap(Map m, V<int> pos, int range) {
        display.fill(Color(0).to_sdl());
        for (int x = 0; x<m)
    }
};