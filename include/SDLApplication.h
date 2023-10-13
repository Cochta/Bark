#pragma once
//
// Created by Coch on 09.10.2023.
//

#include "Circle.h"

#include <SDL2/SDL.h>

#include <vector>

class SDLApplication
{
public:
    Vec2F Center;
    int Width, Height;
    Vec2I MousePos;
    unsigned long long Time = 0;
    std::vector<Circle> circles;

    SDLApplication(std::string_view title, int width, int height);

    ~SDLApplication();

    void Run();

    void AddCircle(const Circle &c);
    void DrawCircle(const Circle &c);
    void DrawAllCircles();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
};