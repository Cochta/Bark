//
// Created by Coch on 09.10.2023.
//
#include "SDLApplication.h"

SDLApplication::SDLApplication(const char *title, int width, int height) : window(nullptr), renderer(nullptr)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        // Handle SDL initialization error
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return;
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        // Handle window creation error
        SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
    {
        // Handle renderer creation error
        SDL_Log("SDL_CreateRenderer Error: %s", SDL_GetError());
        return;
    }
}

SDLApplication::~SDLApplication()
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

void SDLApplication::Run()
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
        SDL_GetMouseState(&mouseX, &mouseY);

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        DrawAllCircles(); // x, y, radius, color

        // Present the renderer
        SDL_RenderPresent(renderer);
    }
}

void SDLApplication::AddCircle(Circle &c)
{
    circles.push_back(c);
}

void SDLApplication::DrawCircle(const Circle &c)
{
    SDL_SetRenderDrawColor(renderer, c.color.r, c.color.g, c.color.b, c.color.a);

    for (int w = 0; w < c.radius * 2; w++)
    {
        for (int h = 0; h < c.radius * 2; h++)
        {
            int dx = c.radius - w; // Horizontal offset
            int dy = c.radius - h; // Vertical offset
            if ((dx * dx + dy * dy) <= (c.radius * c.radius))
            {
                SDL_RenderDrawPoint(renderer, c.x + dx, c.y + dy);
            }
        }
    }
}

void SDLApplication::DrawAllCircles()
{
    if (circles.empty())
        return;
    for (const Circle &circle: circles)
    {
        DrawCircle(circle);
    }
}