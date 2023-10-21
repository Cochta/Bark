//
// Created by Coch on 20.10.2023.
//

#pragma once

#include <iostream>
#include "Sample.h"
#include "Random.h"

class TriggerSample : public Sample
{
    static constexpr std::size_t CIRCLE_NBR = 103;
    static constexpr float SPEED = 50;



    void SetUp() override
    {
        Sample::SetUp();

        for (std::size_t i = 0; i < CIRCLE_NBR; ++i)
        {
            auto bodyRef1 = World.CreateBody();
            BodyRefs.push_back(bodyRef1);
            auto &body1 = World.GetBody(bodyRef1);
            Math::Vec2F v = {Math::Random::Range(-1.f, 1.f), Math::Random::Range(-1.f, 1.f)};
            body1.Velocity = v.Normalized() * SPEED;
            body1.Position = {Math::Random::Range(100.f, Metrics::Width - 100.f),
                              Math::Random::Range(100.f, Metrics::Height - 100.f)};

            auto colRef1 = World.CreateCollider(bodyRef1);
            ColRefs.push_back(colRef1);
            auto &col1 = World.GetCollider(colRef1);
            col1.ColShape = new Shape();
            col1.ColShape->Type = Math::ShapeType::Circle;
            col1.ColShape->Circle = new Math::Circle(body1.Position, Metrics::MetersToPixels(0.3f));

            BodyData bd;
            bd.Shape.Type = col1.ColShape->Type;
            bd.Shape.Circle = col1.ColShape->Circle;
            bd.Color = {0, 0, 255, 255};
            AllBodyData.push_back(bd);

        }
    }

    void Update() override
    {
        for (int i = 0; i < ColRefs.size(); ++i)
        {
            if (World.GetCollider(ColRefs[i]).DoesTrigger())
            {
                AllBodyData[i].Color = {0, 255, 0, 255};
            } else
            {
                AllBodyData[i].Color = {0, 0, 255, 255};
            }
        }
        Sample::Update();
    }

    void TearDown() override
    {

        Sample::TearDown();

    }
};