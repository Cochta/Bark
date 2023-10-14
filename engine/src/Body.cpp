//
// Created by Coch on 10.10.2023.
//
#include "Body.h"

void Body::ApplyForce(const Vec2F &force) noexcept
{
    Force += force;
}

void Body::Update(float deltaTime) noexcept
{
    // Calculate acceleration based on Newton's second law (F = ma)
    Acceleration = Force / Mass;

    // Update velocity and position using the calculated acceleration
    Velocity += Acceleration * deltaTime;
    Position += Velocity * deltaTime;

    // Reset the force for the next frame
    Force = Vec2F(0, 0);
}