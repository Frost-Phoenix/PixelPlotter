#include <stdio.h>

#include "config.h"
#include "pplotter.h"


int main() {

    pplotter p;
    pplotter_init(
        &p,
        "test",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIN_WIDTH * SCALE,
        WIN_HEIGHT * SCALE,
        SDL_WINDOW_SHOWN
    );

    while (p.running) {
        pplotter_poll_events(&p);
        pplotter_update(&p);
        pplotter_render(&p);
    }

    pplotter_quit(&p);

    return 0;
}