//
// Created by const on 13/10/2023.
//
#pragma once

#include "Body.h"
#include "Timer.h"

#include <vector>

struct BodyRef
{
    std::size_t Index;
    std::size_t GenIndex;

    constexpr bool operator==(const BodyRef& other) const
    {
        return (Index == other.Index) && (GenIndex == other.GenIndex);
    }
};

class World
{
private:
    std::vector<Body> _bodies;
public:

    std::vector<size_t> GenIndices;
    Timer Timer;

    World() noexcept = default;

    void Init() noexcept;

    void Update() noexcept;

    [[nodiscard]] BodyRef CreateBody() noexcept;

    [[nodiscard]] Body& GetBody(BodyRef bodyRef);

};