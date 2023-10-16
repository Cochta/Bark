//
// Created by Coch on 10.10.2023.
//
#include "Metrics.h"

#include "Vec2.h"

class Body
{
public:
    Vec2F Position = Vec2F::Zero();
    Vec2F Velocity = Vec2F::Zero();
    float Mass = 1.f;

    Vec2F Force = Vec2F::Zero(); // Total force acting on the body

    explicit Body(Vec2F position, Vec2F velocity = Vec2F::Zero(), float mass = 1.f) noexcept
            : Position(position), Velocity(velocity), Mass(mass)
    {}

    void ApplyForce(const Vec2F &force) noexcept;
};