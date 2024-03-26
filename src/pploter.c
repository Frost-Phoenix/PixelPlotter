#include "pplotter.h"


/******************************************************
 *                 Private functions                  *
 ******************************************************/

static void priv_poll_events(pplotter* p) {
    while (SDL_PollEvent(&p->event) != 0) {
        switch (p->event.type) {
            case SDL_QUIT: p->running = false; break;

            default: break;
        }
    }
}

// static void priv_update(pplotter* p) { }

static void priv_render(pplotter* p) {
    SDL_RenderClear(p->ren);
    SDL_RenderCopy(p->ren, p->texture, NULL, NULL);
    SDL_RenderPresent(p->ren);
}


/******************************************************
 *                 Public functions                   *
 ******************************************************/

void pplotter_init(pplotter* p, const char* title, int x, int y, int w, int h, Uint32 flags) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("[ERROR] Unable to init window: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    p->win = SDL_CreateWindow(title, x, y, w, h, flags);
    if (p->win == NULL) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    p->ren = SDL_CreateRenderer(
        p->win,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (p->win == NULL) {
        SDL_Log("Unable to create renderer: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_RenderSetLogicalSize(p->ren, WIN_WIDTH, WIN_HEIGHT);

    p->texture = SDL_CreateTexture(
        p->ren,
        SDL_PIXELFORMAT_RGB24,                                      /* RR GG BB */
        SDL_TEXTUREACCESS_STREAMING,
        WIN_WIDTH,
        WIN_HEIGHT
    );
    if (p->texture == NULL) {
        SDL_Log("Unable to create texture: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    memset(p->buffer, 0, sizeof(uint8_t) * WIN_WIDTH * WIN_HEIGHT * 3);
    SDL_UpdateTexture(p->texture, NULL, p->buffer, WIN_WIDTH * 3);

    p->running = true;

    SDL_Log("Initialized");
}

void pplotter_quit(pplotter* p) {
    SDL_DestroyWindow(p->win);
    SDL_DestroyRenderer(p->ren);
    SDL_DestroyTexture(p->texture);

    SDL_Quit();

    SDL_Log("Quit");
}

void pplotter_loop(pplotter* p) {
    while (p->running) {
        priv_poll_events(p);
        // priv_update(p);
        priv_render(p);
    }
}