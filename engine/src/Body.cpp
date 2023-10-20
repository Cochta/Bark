//
// Created by Coch on 10.10.2023.
//
#include "Body.h"

void Body::ApplyForce(const Math::Vec2F &force) noexcept
{
    _force += force;
}