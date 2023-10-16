//
// Created by const on 13/10/2023.
//
#include "World.h"

void World::AddBody(const Body &body) noexcept
{
    Bodies.push_back(body);
}

void World::Init() noexcept
{
    Timer.Init();
}

void World::Update() noexcept
{
    Timer.Tick();
    for (auto &body: Bodies)
    {
        auto acceleration = body.Force / body.Mass;
        body.Velocity += acceleration * Timer.DeltaTime;
        body.Position += body.Velocity * Timer.DeltaTime;

        body.Force = Vec2F::Zero();
    }
}
