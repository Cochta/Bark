//
// Created by const on 18/10/2023.
//
#pragma once

#include "World.h"

#include "Timer.h"

enum class BodyType {
    Sun = 0,
    Planet = 1
};
struct Color {
    int r = 255, g = 255, b = 255, a = 255;
};
struct BodyData // for sdl to draw
{
    BodyType Type = BodyType::Planet;
    float Radius = 0; //if radius is 0 then the object is not a circle
    Color Color = {255, 255, 255, 255};

};

class Sample {
public:
    World World;
    Timer Timer;

    std::vector<BodyRef> BodyRefs;
    std::vector<BodyData> AllBodyData;

    virtual void SetUp() {
        World.SetUp();
        Timer.SetUp();
    };

    virtual void Update() {
        Timer.Tick();
        World.Update(Timer.DeltaTime);
    };

};