//
// Created by const on 13/10/2023.
//
#pragma once

#include "Body.h"
#include "Timer.h"

#include <vector>

class World
{
public:
    std::vector<Body> Bodies;
    Timer Timer;

    void Init() noexcept;

    void AddBody(const Body &body) noexcept;

    void Update() noexcept;
};