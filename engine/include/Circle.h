//
// Created by Coch on 10.10.2023.
//
#include "Vec2.h"

struct Color
{
    int r, g, b, a;

    constexpr Color(int red = 0, int green = 0, int blue = 0, int alpha = 255)
            : r(red), g(green), b(blue), a(alpha)
    {}
};

inline constexpr static Color Red = Color(255, 0, 0, 255);
inline constexpr static Color Green = Color(0, 255, 0, 255);
inline constexpr static Color Blue = Color(0, 0, 255, 255);
inline constexpr static Color Black = Color(0, 0, 0, 255);
inline constexpr static Color White = Color(255, 255, 255, 255);

class Circle
{
public:
    Vec2F Position = Vec2F(0, 0);
    Vec2F Velocity = Vec2F(0, 0); // add masse inversée
    float Speed = 5;
    float Radius = 1.f;
    Color Col;

    explicit Circle(Vec2F position, float speed, float radius, Color color = Green) noexcept
            : Position(position), Speed(speed), Radius(radius), Col(color)
    {}

    void Update(float deltaTime) noexcept;

    void RotateAround(Vec2F point, float deltaTime) noexcept;
};