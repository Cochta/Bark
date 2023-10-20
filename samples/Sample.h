//
// Created by const on 18/10/2023.
//
#pragma once

#include "World.h"
#include "Metrics.h"

#include "Timer.h"

enum class BodyType
{
    Sun = 0,
    Planet = 1,
};

struct Color
{
    int r = 255, g = 255, b = 255, a = 255;
};

struct BodyData // for sdl to draw
{
    BodyType Type = BodyType::Planet;
    float Radius = 0; //if radius is 0 then the object is not a Circle
    Color Color = {0, 255, 0, 255};

};

class Sample
{
public:
    World World;
    Timer Timer;

    std::vector<BodyRef> BodyRefs;
    std::vector<BodyData> AllBodyData;

    virtual void SetUp()
    {
        Timer.SetUp();
        World.SetUp();

    };

    virtual void Update()
    {
        Timer.Tick();
        World.Update(Timer.DeltaTime);
    };
};
