//
// Created by const on 18/10/2023.
//
#pragma once

#include "World.h"
#include "Metrics.h"

#include "Timer.h"

struct Color
{
    int r = 255, g = 255, b = 255, a = 255;
};

struct BodyData // for sdl to draw
{
    std::variant<Math::CircleF, Math::RectangleF, Math::PolygonF> Shape{
            Math::CircleF(Math::Vec2F::Zero(), 1)};
    Color Color = {0, 255, 0, 255};
};

class Sample
{
protected:

    Timer _timer;

    Math::Vec2F _mousePos;




public:
    World World;

    std::vector<BodyRef> BodyRefs;
    std::vector<ColliderRef> ColRefs;
    std::vector<BodyData> AllBodyData;

    virtual void SetUp()
    {
        _timer.SetUp();
        World.SetUp();
    };

    virtual void Update()
    {
        _timer.Tick();
        World.Update(_timer.DeltaTime);
    };

    virtual void TearDown()
    {
        BodyRefs.clear();
        AllBodyData.clear();
        ColRefs.clear();
        World.TearDown();

    };

    virtual void GetMousePos(Math::Vec2F mousePos)
    {
        _mousePos = mousePos;
    }

};
