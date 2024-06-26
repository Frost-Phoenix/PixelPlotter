#include "pplotter.h"


/******************************************************
 *                 Private functions                  *
 ******************************************************/




 /******************************************************
  *                 Public functions                   *
  ******************************************************/

void pplotter_init(pplotter_t* p, const char* title, int x, int y, int w, int h, const Uint32 flags) {
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
    SDL_UpdateTexture(p->texture, NULL, p->buffer, 0);

    p->running = true;

    SDL_Log("Initialized");
}

void pplotter_quit(pplotter_t* p) {
    SDL_DestroyWindow(p->win);
    SDL_DestroyRenderer(p->ren);
    SDL_DestroyTexture(p->texture);

    SDL_Quit();

    SDL_Log("Quit");
}

void pplotter_set_bg_color(pplotter_t* p, uint8_t r, uint8_t g, uint8_t b) {
    for (size_t i = 0; i < WIN_HEIGHT * WIN_WIDTH * 3; i += 3) {
        p->buffer[i] = r;
        p->buffer[i + 1] = g;
        p->buffer[i + 2] = b;
    }
}

int pplotter_set_pixel(pplotter_t* p, int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    if (!(0 <= x && x <= WIN_WIDTH && 0 <= y && y <= WIN_HEIGHT)) {
        SDL_Log("Can't draw pixel at pos %d - %d", x, y);
        return -1;
    }

    int i = (y * WIN_WIDTH + x) * 3;

    p->buffer[i] = r;
    p->buffer[i + 1] = g;
    p->buffer[i + 2] = b;

    return 0;
}

void pplotter_poll_events(pplotter_t* p) {
    while (SDL_PollEvent(&p->event) != 0) {
        switch (p->event.type) {
            case SDL_QUIT:
                p->running = false;
                break;
            default:
                break;
        }
    }
}

void pplotter_update(pplotter_t* p) {
    SDL_UpdateTexture(p->texture, NULL, p->buffer, WIN_WIDTH * 3);
}

void pplotter_render(pplotter_t* p) {
    SDL_RenderClear(p->ren);
    SDL_RenderCopy(p->ren, p->texture, NULL, NULL);
    SDL_RenderPresent(p->ren);
}