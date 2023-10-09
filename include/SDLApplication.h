#pragma once
//
// Created by Coch on 09.10.2023.
//
#include <SDL2/SDL.h>
#include <vector>

struct Color
{
    int r,g,b,a;
    Color(int red = 0, int green = 0, int blue = 0, int alpha = 255)
            : r(red), g(green), b(blue), a(alpha) {}
};
inline const static Color Red = Color(255, 0, 0, 255);
inline const static Color Green = Color(0, 255, 0, 255);
inline const static Color Blue = Color(0, 0, 255, 255);
inline const static Color Black = Color(0, 0, 0, 255);
inline const static Color White = Color(255, 255, 255, 255);

struct Circle
{
    int x;
    int y;
    int radius;
    Color color;

    Circle(int _x, int _y, int _radius, Color _color)
            : x(_x), y(_y), radius(_radius), color(_color)
    {}
};


class SDLApplication
{
public:
    int mouseX, mouseY;
    std::vector<Circle> circles;

    SDLApplication(const char *title, int width, int height);

    ~SDLApplication();

    void Run();

    void AddCircle(Circle &c);
    void DrawCircle(const Circle &c);
    void DrawAllCircles();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
};