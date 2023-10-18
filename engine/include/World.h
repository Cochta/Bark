//
// Created by const on 13/10/2023.
//
#pragma once

#include "Body.h"

#include <vector>

// todo: add brief to all engine classes, funcs, attribute
struct BodyRef {
    std::size_t Index;
    std::size_t GenIndex;

    constexpr bool operator==(const BodyRef &other) const {
        return (Index == other.Index) && (GenIndex == other.GenIndex);
    }
};

class World {
private:
    std::vector<Body> _bodies;

public:
    std::vector<size_t> GenIndices;

    World() noexcept = default;

    void SetUp() noexcept;

    void Update(float deltaTime) noexcept;

    [[nodiscard]] BodyRef CreateBody() noexcept;

    void DestroyBody(BodyRef bodyRef);

    [[nodiscard]] Body &GetBody(BodyRef bodyRef);
};