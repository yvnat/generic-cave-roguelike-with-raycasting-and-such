#pragma once

class Renderer {
    public:
    Display display;
    int scale = 1;
    Renderer(Display d) {
        this->display = d;
    }
    void renderMap(Map m, V<int> pos, int radius) {
        display.fill(Color(1).to_sdl());
        for (int x = pos.x-radius; x<pos.x+radius; x++) {
            for (int y = pos.y-radius; y<pos.y+radius; y++) {
                V<float> dir = (
                    (V<float>(pos.x,pos.y)-V<float>(x,y)) + V<float>(0.5,0.5)
                ).normalized();

                RaycastHit *hit = m.castRay(pos,dir,radius);
                if (hit != nullptr) {
                    Tile col = hit->collided;
                    float distance = hit->distance;
                    if (&col == &m.map[y][x]) {
                        display.draw_rect({x,y,1,1},Color(0).to_sdl());
                    }
                    
                } else {
                    // Don't render tile at (x,y)
                }
            }
        }
    }
};