#ifndef PPLOTTER_H
#define PPLOTTER_H

#include <stdbool.h>
#include <SDL2/SDL.h>

#include "config.h"


typedef struct pplotter {
    SDL_Window* win;
    SDL_Renderer* ren;

    uint8_t buffer[WIN_WIDTH * WIN_HEIGHT * 3];
    SDL_Texture* texture;

    bool running;
    SDL_Event event;
} pplotter;


void pplotter_init(pplotter* p, const char* title, int x, int y, int w, int h, Uint32 flags);
void pplotter_quit(pplotter* p);

void pplotter_set_pixel(pplotter* p, int x, int y, uint8_t r, uint8_t g, uint8_t b);

void pplotter_poll_events(pplotter* p);
void pplotter_update(pplotter* p);
void pplotter_render(pplotter* p);


#endif /* PPLOTTER_H */