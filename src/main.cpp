#include <SDL2/SDL.h>
#include <iostream>
#include "euler.h"

int main(int argc, char *argv[]) {

    std::cout << GetEulerNumber() << std::endl;
    std::cout << GetEulerNumber() << std::endl;
    std::cout << GetEulerNumber() << std::endl;
//    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
//        return -1;
//    }
//
//    SDL_Window* window = nullptr;
//    window = SDL_CreateWindow(
//            "SDL2 Window",        // Window title
//            SDL_WINDOWPOS_UNDEFINED, // X position of the window (centered)
//            SDL_WINDOWPOS_UNDEFINED, // Y position of the window (centered)
//            800, 600,              // Width and height of the window
//            SDL_WINDOW_SHOWN       // Window flags (e.g., SDL_WINDOW_SHOWN, SDL_WINDOW_FULLSCREEN)
//    );
//
//    if (window == nullptr) {
//        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
//        SDL_Quit();
//        return -1;
//    }
//
//    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//    if (renderer == nullptr) {
//        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
//        SDL_DestroyWindow(window);
//        SDL_Quit();
//        return -1;
//    }
//
//    bool quit = false;
//    SDL_Event e;
//
//    while (!quit) {
//        while (SDL_PollEvent(&e) != 0) {
//            if (e.type == SDL_QUIT) {
//                quit = true;
//            }
//        }
//
//        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red (R, G, B, A)
//        SDL_RenderClear(renderer);
//
//        // Add your rendering code here
//
//        // Present the renderer
//        SDL_RenderPresent(renderer);
//
//        // Optionally, you can add a delay to control frame rate
//        SDL_Delay(16); // 60 frames per second
//    }
//
//    SDL_DestroyWindow(window);
//    SDL_Quit();

    return 0;
}