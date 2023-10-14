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

    Vec2F Force = Vec2F(0, 0); // Total force acting on the body
    Vec2F Acceleration = Vec2F(0, 0); // Acceleration of the body

    explicit Body(Vec2F position, Vec2F velocity = Vec2F::Zero(), float mass = 1.f) noexcept
            : Position(position), Velocity(velocity), Mass(mass)
    {}

    void ApplyForce(const Vec2F& force) noexcept;

    void Update(float deltaTime) noexcept;
};