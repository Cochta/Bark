#pragma once
//
// Created by Coch on 09.10.2023.
//

#include "Body.h"

#include <SDL2/SDL.h>

#include <vector>

class SDLApp
{
public:
    std::string_view Title;
    int Width, Height;
    Vec2I MousePos;
    unsigned long long Time = 0;
    std::vector<Body> Bodies;

    SDLApp(std::string_view title, int width, int height);

    void Init();

    void UnInit();

    void Run();

    void AddCircle(const Body &c);
    void DrawCircle(const Body &c);
    void DrawAllCircles();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
};