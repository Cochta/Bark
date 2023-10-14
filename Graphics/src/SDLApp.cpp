//
// Created by Coch on 09.10.2023.
//
#include "SDLApp.h"

SDLApp::SDLApp(std::string_view title, int width, int height, World &world) : window(nullptr), renderer(nullptr),
                                                                              GameWorld(world)
{
    Width = width;
    Height = height;
    Title = title;
}

void SDLApp::Init()
{

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        // Handle SDL initialization error
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return;
    }
    window = SDL_CreateWindow(Title.data(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Width, Height,
                              SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        // Handle window creation error
        SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
        return;
    }
    // Enable VSync
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
    {
        // Handle renderer creation error
        SDL_Log("SDL_CreateRenderer Error: %s", SDL_GetError());
        return;
    }

}

void SDLApp::UnInit()
{
    if (renderer != nullptr)
    {
        SDL_DestroyRenderer(renderer);
    }
    if (window != nullptr)
    {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

void SDLApp::Run()
{
    bool quit = false;
    SDL_Event e;

    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        SDL_GetMouseState(&MousePos.X, &MousePos.Y);

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);









        GameWorld.Update();

        DrawAllBodies();

        // Present the renderer
        SDL_RenderPresent(renderer);
    }
}


void SDLApp::DrawCircle(const Body &c)
{
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    int Radius = 15;
    for (int w = 0; w < Radius * 2; w++)
    {
        for (int h = 0; h < Radius * 2; h++)
        {
            float dx = Radius - w; // Horizontal offset
            float dy = Radius - h; // Vertical offset
            if ((dx * dx + dy * dy) <= (Radius * Radius))
            {
                SDL_RenderDrawPoint(renderer, c.Position.X + dx, c.Position.Y + dy);
            }
        }
    }
}

void SDLApp::DrawAllBodies()
{
    if (GameWorld.Bodies.empty())
        return;
    for (Body &body: GameWorld.Bodies)
    {
        DrawCircle(body);
    }
}
