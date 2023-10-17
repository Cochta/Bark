#pragma once
//
// Created by Coch on 09.10.2023.
//

#include "World.h"

#include <SDL2/SDL.h>

#include <vector>

class SDLApp
{
public:
    std::string_view Title;
    int Width, Height;
    Vec2I MousePos;

    World GameWorld;


    SDLApp(std::string_view title, int width, int height);

    void Init();

    void UnInit();

    void SDLUpdate();

    void Run();

    void DrawCircle(const Body &c, float radius, int segments, SDL_Color col);

    void DrawAllBodies();

private:
    SDL_Window *window{};
    SDL_Renderer *renderer{};
};