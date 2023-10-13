//
// Created by Coch on 10.10.2023.
//
#include "Body.h"

void Body::Update(float deltaTime) noexcept
{
    Position += Velocity * deltaTime;
}

void Body::RotateAround(Vec2F point, float deltaTime) noexcept
{
    Vec2F centerToPoint = point - Position; // Rotation Center
    Velocity = Vec2F(-centerToPoint.Y, centerToPoint.X).Normalized() * Speed;
    Vec2F accel = (Velocity * Velocity) / centerToPoint.Length();
    Velocity += accel.Normalized() * Speed * deltaTime;
}