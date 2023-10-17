//
// Created by Coch on 09.10.2023.
//
#include <array>
#include "SDLApp.h"

SDLApp::SDLApp(std::string_view title, int width, int height) : window(nullptr), renderer(nullptr)
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


    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
    {
        // Handle renderer creation error
        SDL_Log("SDL_CreateRenderer Error: %s", SDL_GetError());
        return;
    }
    GameWorld.Init();
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
        SDLUpdate();

        DrawAllBodies();

        // Present the renderer
        SDL_RenderPresent(renderer);
    }
}

void SDLApp::DrawCircle(const Body &b, float radius, int segments, SDL_Color col)
{
    std::vector<SDL_Vertex> vertices;
    std::vector<int> indices;

    // Calculate vertices for the circle
    for (int i = 0; i < segments; ++i)
    {
        Radian angle = Radian(2 * MathUtility::Pi * i / segments);
        float x = b.Position.X + radius * MathUtility::Cos(angle);
        float y = b.Position.Y + radius * MathUtility::Sin(angle);
        vertices.push_back({{x, y}, col, {1.0f, 1.0f}});
    }

    // Calculate indices to create triangles for filling the circle
    for (int i = 0; i < segments - 1; ++i)
    {
        indices.push_back(0); // Center point
        indices.push_back(i);
        indices.push_back(i + 1);
    }
    indices.push_back(0); // Center point
    indices.push_back(segments - 1);
    indices.push_back(0);  // Connect the last vertex to the center

    SDL_RenderGeometry(renderer, nullptr, vertices.data(), vertices.size(), indices.data(), indices.size());
}


