//
// Created by Coch on 10.10.2023.
//
#include "Vec2.h"

class Body
{
public:
    Vec2F Position = Vec2F(0, 0);
    Vec2F Velocity = Vec2F(0, 0); // add masse inversée
    float Mass = 1.f;

    explicit Body(Vec2F position, Vec2F velocity = Vec2F::Zero(), float mass = 1.f) noexcept
            : Position(position), Velocity(velocity), Mass(mass)
    {}

    void Update(float deltaTime) noexcept;

    void RotateAround(Vec2F point, float deltaTime) noexcept;
};