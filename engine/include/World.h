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
    Timer WorldTimer;

    Body WorldCenter = Body({300, 400}, Vec2F::Zero(), 100);

    void Init();

    void UnInit();

    void AddBody(const Body &body);

    void Update();
};