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

    void Init();
    void UnInit();

    void AddBody(const Body &body);

    void Update();
};