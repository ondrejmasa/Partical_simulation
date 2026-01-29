# pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "app.h"
#include "global.h"

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window* pWindow;
    SDL_Renderer* pRenderer;
    SDL_CreateWindowAndRenderer("Particle simulation", gbl::WIDTH, gbl::HEIGHT, 0, &pWindow, &pRenderer);
    SDL_SetRenderVSync(pRenderer, 1);
    SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_BLEND);
    if (!pWindow) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    App app;
	Uint64 lastTime = SDL_GetTicks();

    // Main loop
    SDL_Event event;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

		Uint64 currentTime = SDL_GetTicks();
        float deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
        SDL_RenderClear(pRenderer);

        app.Update(deltaTime);
        app.Render(pRenderer);

        SDL_RenderPresent(pRenderer);
       
    }


    // Cleanup
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return 0;
}

