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

    std::vector<ColliderRef> ColRefs;

    void SetUp() override
    {
        Sample::SetUp();
        auto bodyRef1 = World.CreateBody();
        BodyRefs.push_back(bodyRef1);
        auto &body1 = World.GetBody(bodyRef1);
        body1.Velocity = Math::Vec2F::Right() * SPEED;
        body1.Position = {100.f, 300.f};

        auto colRef1 = World.CreateCollider(bodyRef1);
        ColRefs.push_back(colRef1);
        auto &col1 = World.GetCollider(colRef1);
        col1.ColShape = new Shape();
        col1.ColShape->Type = Math::ShapeType::Circle;
        col1.ColShape->Circle = new Math::Circle(body1.Position, Metrics::MetersToPixels(1.f));

        BodyData sbd{BodyType::Planet,
                     Metrics::MetersToPixels(1.f),
                     {0, 0, 255, 255}};
        AllBodyData.push_back(sbd);

        auto bodyRef2 = World.CreateBody();
        BodyRefs.push_back(bodyRef2);
        auto &body2 = World.GetBody(bodyRef2);
        body2.Velocity = Math::Vec2F::Left() * SPEED;
        body2.Position = {500.f, 300.f};

        auto colRef2 = World.CreateCollider(bodyRef2);
        ColRefs.push_back(colRef2);
        auto &col2 = World.GetCollider(colRef2);
        col2.ColShape = new Shape();
        col2.ColShape->Type = Math::ShapeType::Circle;
        col2.ColShape->Circle = new Math::Circle(body2.Position, Metrics::MetersToPixels(1.f));
        BodyData sbd2{BodyType::Planet,
                      Metrics::MetersToPixels(1.f),
                      {0, 0, 255, 255}};
        AllBodyData.push_back(sbd2);


        // for (std::size_t i = 0; i < CIRCLE_NBR; ++i)
        //  {
//            auto bodyRef1 = World.CreateBody();
//            BodyRefs.push_back(bodyRef1);
//            auto &body1 = World.GetBody(bodyRef1);
//            body1.Velocity = {Math::Random::Range(-1.f, 1.f) * SPEED, Math::Random::Range(-1.f, 1.f) * SPEED};
//            body1.Position = {Math::Random::Range(100.f, Metrics::Width - 100.f),
//                             Math::Random::Range(100.f, Metrics::Height - 100.f)};
//
//            auto colRef1 = World.CreateCollider(bodyRef1);
//            ColRefs.push_back(colRef1);
//            auto &col1 = World.GetCollider(colRef1);
//            col1.ColShape = new Shape();
//            col1.ColShape->Type = Math::ShapeType::Circle;
//            col1.ColShape->Circle = new Math::Circle(body1.Position, Metrics::MetersToPixels(0.1f));
//
//            BodyData sbd{BodyType::Planet,
//                         Metrics::MetersToPixels(0.1f),
//                         {0, 0, 255, 255}};
//            AllBodyData.push_back(sbd);

        //   }
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
};