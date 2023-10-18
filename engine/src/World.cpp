//
// Created by const on 13/10/2023.
//
#include "World.h"

void World::SetUp() noexcept
{
    int initSize = 100;

    _bodies.resize(initSize, Body());
    GenIndices.resize(initSize, 0);
}

void World::Update(float deltaTime) noexcept
{
    for (auto &body: _bodies)
    {
        if (!body.IsEnabled()) continue;
        auto acceleration = body.GetForce() / body.Mass;
        body.Velocity += acceleration * deltaTime;
        body.Position += body.Velocity * deltaTime;

        body.ResetForce();
    }
}

[[nodiscard]] BodyRef World::CreateBody() noexcept
{
    auto it = std::find_if(_bodies.begin(), _bodies.end(), [](const Body &body)
    {
        return !body.IsEnabled(); // Get first Disabled body
    });

    if (it != _bodies.end())
    {
        std::size_t index = std::distance(_bodies.begin(), it);
        auto bodyRef = BodyRef{index, GenIndices[index]};
        GetBody(bodyRef).Enable();
        return bodyRef;
    }

    std::size_t previousSize = _bodies.size();

    _bodies.resize(previousSize * 2, Body());
    GenIndices.resize(previousSize * 2, 0);

    BodyRef bodyRef = {previousSize, GenIndices[previousSize]};
    GetBody(bodyRef).Enable();
    return bodyRef;

}

void World::DestroyBody(BodyRef bodyRef)
{
    if (GenIndices[bodyRef.Index] != bodyRef.GenIndex)
    {
        throw std::runtime_error("No body found !");
    }

    _bodies[bodyRef.Index].Disable();
}

[[nodiscard]] Body &World::GetBody(BodyRef bodyRef)
{
    if (GenIndices[bodyRef.Index] != bodyRef.GenIndex)
    {
        throw std::runtime_error("No body found !");
    }

    return _bodies[bodyRef.Index];
}

