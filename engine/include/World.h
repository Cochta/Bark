//
// Created by const on 13/10/2023.
//
#pragma once

#include "Body.h"
#include "Timer.h"

#include <vector>

struct BodyRef
{
    size_t Index;
};

class World
{
public:
    std::vector<Body> Bodies;
    std::vector<size_t> Indices;
    Timer Timer;

    void Init() noexcept;

    void Update() noexcept;

    void AddBody(const Body &body) noexcept;
};