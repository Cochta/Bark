//
// Created by Coch on 10.10.2023.
//
#include "Circle.h"

void Circle::Update(float deltaTime) noexcept
{
    Position += Velocity * deltaTime;
}

void Circle::RotateAround(Vec2F point, float deltaTime) noexcept
{
    Vec2F centerToPoint = point - Position; // Rotation Center
    Velocity = Vec2F(-centerToPoint.Y, centerToPoint.X).Normalized() * Speed;
    Vec2F accel = (Velocity * Velocity) / centerToPoint.Length();
    Velocity += accel.Normalized() * Speed * deltaTime;

//    Vec2F centerToPoint = Position - point; // Rotation Center
//    float angularSpeed = 2 * MathUtility::Pi / 10 * deltaTime;
//    float distanceToCenter = centerToPoint.Length();
//    Vec2F tangentialVelocity = Vec2F(-centerToPoint.Y, centerToPoint.X).Normalized() * angularSpeed * distanceToCenter;
//    Velocity = tangentialVelocity * Speed;

//        Vec2F centerToPoint = Position - point;
//        float angularSpeed = 2 * MathUtility::Pi / 10 * deltaTime;
//
//        float newX = centerToPoint.X * cos(angularSpeed) - centerToPoint.Y * sin(angularSpeed);
//        float newY = centerToPoint.X * sin(angularSpeed) + centerToPoint.Y * cos(angularSpeed);
//
//        Position = Vec2F(newX, newY) + point;
}