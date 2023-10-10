//
// Created by Coch on 09.10.2023.
//
#include "SDLApplication.h"

SDLApplication::SDLApplication(const char *title, int width, int height) : window(nullptr), renderer(nullptr)
{
    Width = width;
    Height = height;
    Center = {static_cast<float>(Width) / 2, static_cast<float>(Height) / 2};
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
    // Enable VSync
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
    // Au start de la window
    Time = SDL_GetTicks64();

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
    // Dans l'update
    const float deltaTime = (SDL_GetTicks64() - Time) / 1000.f;

    Time = SDL_GetTicks64();

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


        for (auto &c: circles)
        {
            Vec2F centerToPlanet = c.Position - Center; // Rotation Center
            float distanceToCenter = centerToPlanet.Length(); // Rayon
            float angularSpeed = 2 * MathUtility::Pi / 10 * deltaTime; // Vitesse angulaire

            Vec2F tangentialVelocity = Vec2F(-centerToPlanet.Y, centerToPlanet.X).Normalized() * angularSpeed * distanceToCenter;

            c.Velocity = tangentialVelocity;

            c.Update(deltaTime);
        }

        DrawAllCircles(); // x, y, Radius, Color


        // Present the renderer
        SDL_RenderPresent(renderer);
    }
}

void SDLApplication::AddCircle(Circle &c)
{
    circles.push_back(c);
}

void SDLApplication::DrawCircle(Circle &c)
{
    SDL_SetRenderDrawColor(renderer, c.Col.r, c.Col.g, c.Col.b, c.Col.a);

    for (int w = 0; w < c.Radius * 2; w++)
    {
        for (int h = 0; h < c.Radius * 2; h++)
        {
            int dx = c.Radius - w; // Horizontal offset
            int dy = c.Radius - h; // Vertical offset
            if ((dx * dx + dy * dy) <= (c.Radius * c.Radius))
            {
                SDL_RenderDrawPoint(renderer, c.Position.X + dx, c.Position.Y + dy);
            }
        }
    }
}

void SDLApplication::DrawAllCircles()
{
    if (circles.empty())
        return;
    for (Circle &circle: circles)
    {
        DrawCircle(circle);
    }
}