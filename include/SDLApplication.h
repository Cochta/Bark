#pragma once
//
// Created by Coch on 09.10.2023.
//
#include <SDL2/SDL.h>
#include <vector>
#include "../libs/Math922/include/Vec2.h"

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
    Vec2F Position = Vec2F(0, 0);
    Vec2F Velocity = Vec2F(0, 0);
    int Radius = 1;
    Color Col;

    Circle(Vec2F position, Vec2F velocity = Vec2F::Zero, int radius = 1, Color color = Green) noexcept
    : Position(position), Velocity(velocity), Radius(radius), Col(color) {}

    void Update(float deltaTime) noexcept
    {
        Position += Velocity * deltaTime;
    }
    void RotateAround(Vec2F point, float deltaTime)
    {
        Vec2F centerToPoint = Position - point; // Rotation Center
        float angularSpeed = 2 * MathUtility::Pi / 10 * deltaTime;
        float distanceToCenter = centerToPoint.Length();
        Vec2F tangentialVelocity = Vec2F(-centerToPoint.Y, centerToPoint.X).Normalized() * angularSpeed * distanceToCenter;
        Velocity = tangentialVelocity;
    }
};


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