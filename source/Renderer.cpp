#include "Graphics.cpp"
#include "Utils.cpp"

class Renderer {
    public:
    Display display;
    Renderer(Display d) {
        this->display = d;
    }
    void renderMap() {
        display.fill(Color(0).to_sdl());
        
    }
};