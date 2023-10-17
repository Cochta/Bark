//
// Created by const on 13/10/2023.
//
#include "World.h"

void World::Init() noexcept {
    int initSize = 100;
    Timer.Init();

    _bodies.resize(initSize, Body());
    GenIndices.resize(initSize, 0);
}

void World::Update() noexcept {
    Timer.Tick();
    for (auto &body: _bodies) {
        if (!body.IsEnabled()) continue;
        auto acceleration = body.Force / body.Mass;
        body.Velocity += acceleration * Timer.DeltaTime;
        body.Position += body.Velocity * Timer.DeltaTime;

        body.Force = Vec2F::Zero();
    }
}

[[nodiscard]] BodyRef World::CreateBody() noexcept {
    auto it = std::find_if(_bodies.begin(), _bodies.end(), [](const Body &body) {
        return !body.IsEnabled(); // Get first Disabled body
    });

    if (it != _bodies.end()) {
        std::size_t index = std::distance(_bodies.begin(), it);
        auto bodyRef = BodyRef{index, GenIndices[index]};
        GetBody(bodyRef).Mass = 1.f;
        return bodyRef;
    }

    std::size_t previousSize = _bodies.size();

    _bodies.resize(previousSize * 2);
    GenIndices.resize(previousSize * 2);

    std::fill(_bodies.begin() + previousSize, _bodies.end(), Body());
    std::fill(GenIndices.begin() + previousSize, GenIndices.end(), 0);

    BodyRef bodyRef = {previousSize, GenIndices[previousSize]};
    GetBody(bodyRef).Mass = 1.f;
    return bodyRef;

}

[[nodiscard]] Body &World::GetBody(BodyRef bodyRef) {
    if (GenIndices[bodyRef.Index] != bodyRef.GenIndex) {
        throw std::runtime_error("No body found !");
    }

    return _bodies[bodyRef.Index];
}
