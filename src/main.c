#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#define SCALE       4
#define WIN_WIDTH   160
#define WIN_HEIGHT  144


typedef struct App {
    SDL_Window* win;
    SDL_Renderer* ren;

    uint8_t buffer[WIN_WIDTH * WIN_HEIGHT * 3];
    SDL_Texture* texture;

    bool running;
    SDL_Event events;
} App;


void init(App* app) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("[ERROR] Unable to init window: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    app->win = SDL_CreateWindow(
        "Pixel ploter",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIN_WIDTH * SCALE,
        WIN_HEIGHT * SCALE,
        SDL_WINDOW_SHOWN
    );
    if (app->win == NULL) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    app->ren = SDL_CreateRenderer(
        app->win,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (app->win == NULL) {
        SDL_Log("Unable to create renderer: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_RenderSetLogicalSize(app->ren, WIN_WIDTH, WIN_HEIGHT);

    app->texture = SDL_CreateTexture(
        app->ren,
        SDL_PIXELFORMAT_RGB24,                                      /* RR GG BB */
        SDL_TEXTUREACCESS_STREAMING,
        WIN_WIDTH,
        WIN_HEIGHT
    );
    if (app->texture == NULL) {
        SDL_Log("Unable to create texture: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    memset(app->buffer, 0, sizeof(uint8_t) * WIN_WIDTH * WIN_HEIGHT * 3);

    app->running = true;

    SDL_Log("Initialized");
}

void quit(App* app) {
    SDL_DestroyWindow(app->win);
    SDL_DestroyRenderer(app->ren);
    SDL_DestroyTexture(app->texture);

    SDL_Quit();

    SDL_Log("Quit");
}

int main() {

    App app;

    init(&app);

    SDL_UpdateTexture(app.texture, NULL, app.buffer, WIN_WIDTH * 3);

    while (app.running) {
        while (SDL_PollEvent(&app.events) != 0) {
            if (app.events.type == SDL_QUIT) {
                app.running = false;
            }
        }

        SDL_RenderClear(app.ren);
        SDL_RenderCopy(app.ren, app.texture, NULL, NULL);
        SDL_RenderPresent(app.ren);
    }

    quit(&app);

    return 0;
}