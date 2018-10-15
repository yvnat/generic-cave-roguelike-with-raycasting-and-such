#pragma once

class Renderer {
    public:
    Display display;
    int scale = 1;
    Renderer(Display d) {
        this->display = d;
    }
    void renderMap(Map m, V<int> pos, int radius) {
        //display.fill(Color(1).to_sdl());
        for (int x = pos.x-radius; x<pos.x+radius; x++) {
            for (int y = pos.y-radius; y<pos.y+radius; y++) {
                if (!m.map[y][x].transparent) {
                    display.draw_rect({x,y,1,1},Color(1).to_sdl());
                }
                if (x == pos.x && y == pos.y) {
                    continue;
                }
                V<float> dir = (
                    (V<float>(pos.x,pos.y)-V<float>(x,y))
                ).normalized();

                RaycastHit *hit = m.castRay(display,pos,dir,radius);
                if (hit != nullptr) {
                    display.draw_rect({x,y,1,1},Color(1,0,0).to_sdl());
                    Tile col = hit->collided;
                    float distance = hit->distance;
                    std::cout<<distance<<"\n";
                    if (&col == &m.map[y][x]) {
                        display.draw_rect({x,y,1,1},Color(0,0,1).to_sdl());
                    }
                    
                } else {
                    // don't render
                }
            }
        }
    }
};