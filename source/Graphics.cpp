#pragma once
#include <SDL2/SDL.h>
#include <vector>

using namespace std;

class Display {
public:
    int screen_w;
    int screen_h;

    SDL_Window *window;
    SDL_Surface screen;
    SDL_Renderer *renderer;

    void video_init() {
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow("the greatest roguelike ever", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_w, screen_h, SDL_WINDOW_SHOWN );
        renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
    }
    Display() = default;
    Display(int screen_w, int screen_h) {
        this->screen_w = screen_w;
        this->screen_h = screen_h;
    }
    
    void refresh () {
        SDL_RenderPresent( renderer );
        SDL_UpdateWindowSurface( window );
    }

    vector<SDL_Event> get_events () {
        SDL_Event e;
        vector<SDL_Event> events;
        while( SDL_PollEvent( &e ) != 0 )
        {
            events.push_back(e);
        }
        return events;
    }
    void draw_rect(SDL_Rect r, SDL_Color color, int alpha=255) {
        // SDL_Rect r={rect[0],rect[1],rect[2],rect[3]};
        SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, alpha );
        SDL_RenderFillRect( renderer, &r);

    }
    void fill(SDL_Color color) {
        SDL_SetRenderDrawColor( renderer, color.r,color.g,color.b, 0xFF );
        SDL_RenderClear( renderer );
    }
    void heatmap(vector<vector<float>> map) {

    }
};
