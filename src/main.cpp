#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = nullptr;
    window = SDL_CreateWindow(
            "SDL2 Window",        // Window title
            SDL_WINDOWPOS_UNDEFINED, // X position of the window (centered)
            SDL_WINDOWPOS_UNDEFINED, // Y position of the window (centered)
            800, 600,              // Width and height of the window
            SDL_WINDOW_SHOWN       // Window flags (e.g., SDL_WINDOW_SHOWN, SDL_WINDOW_FULLSCREEN)
    );

    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Optionally, you can add a delay to control frame rate
        SDL_Delay(16); // 60 frames per second
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}