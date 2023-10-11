#pragma once
//
// Created by Coch on 09.10.2023.
//
#include <SDL2/SDL.h>
#include <vector>
#include "Circle.h"
class SDLApplication
{
public:
    Vec2F Center;
    int Width, Height;
    Vec2I MousePos;
    int Time = 0;
    std::vector<Circle> circles;

    SDLApplication(const char *title, int width, int height);

    ~SDLApplication();

    void Run();

    void AddCircle(Circle &c);
    void DrawCircle(Circle &c);
    void DrawAllCircles();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
};