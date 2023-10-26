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

struct BodyData // for the graphics renderer to draw
{
    std::variant<Math::CircleF, Math::RectangleF, Math::PolygonF> Shape{
            Math::CircleF(Math::Vec2F::Zero(), 1)};
    Color Color;
};

class Sample
{
private:
    Timer _timer;

protected:
    std::vector<BodyRef> _bodyRefs;
    std::vector<ColliderRef> _colRefs;

    Math::Vec2F _mousePos;

public:
    World World;

    std::vector<BodyData> AllBodyData;

    void SetUp() noexcept
    {
        _timer.SetUp();
        World.SetUp();
        SampleSetUp();
    };

    void Update() noexcept
    {
        SampleUpdate();
        _timer.Tick();
        World.Update(_timer.DeltaTime);
    };

    virtual void TearDown() noexcept
    {
        _bodyRefs.clear();
        AllBodyData.clear();
        _colRefs.clear();
        World.TearDown();
    };

    virtual void GetMousePos(Math::Vec2F mousePos) noexcept
    {
        _mousePos = mousePos;
    }

    virtual ~Sample() noexcept = default;

protected:
    virtual void SampleUpdate() noexcept = 0;

    virtual void SampleSetUp() noexcept = 0;
};

