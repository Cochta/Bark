//
// Created by Coch on 20.10.2023.
//

#pragma once

#include <iostream>
#include "Sample.h"
#include "Random.h"


class TriggerSample : public Sample, public ContactListener
{
    static constexpr std::size_t CIRCLE_NBR = 50;
    static constexpr float SPEED = 200;

    std::vector<int> triggerNbrPerCollider;

    void BeginContact(ColliderRef &col1, ColliderRef &col2) noexcept override
    {
        triggerNbrPerCollider[col1.Index]++;
        triggerNbrPerCollider[col2.Index]++;

    }

    void EndContact(ColliderRef &col1, ColliderRef &col2) noexcept override
    {
        triggerNbrPerCollider[col1.Index]--;
        triggerNbrPerCollider[col2.Index]--;
    }


    void SetUp() override
    {
        Sample::SetUp();

        World.SetContactListener(this);
        triggerNbrPerCollider.resize(CIRCLE_NBR, 0);

        for (std::size_t i = 0; i < CIRCLE_NBR; ++i)
        {
            auto bodyRef1 = World.CreateBody();
            _bodyRefs.push_back(bodyRef1);
            auto &body1 = World.GetBody(bodyRef1);

            body1.Velocity = Math::Vec2F(Math::Random::Range(-1.f, 1.f),
                                         Math::Random::Range(-1.f, 1.f)) * SPEED;

            body1.Position = {Math::Random::Range(100.f, Metrics::Width - 100.f),
                              Math::Random::Range(100.f, Metrics::Height - 100.f)};

            auto colRef1 = World.CreateCollider(bodyRef1);
            _colRefs.push_back(colRef1);
            auto &col1 = World.GetCollider(colRef1);
            col1.Shape = Math::Circle(Math::Vec2F::Zero(), Metrics::MetersToPixels(0.3f));

            BodyData bd;
            bd.Shape = col1.Shape;
            AllBodyData.push_back(bd);

        }
    }

    void Update() override
    {
        for (std::size_t i = 0; i < _bodyRefs.size(); ++i)
        {
            auto &body = World.GetBody(_bodyRefs[i]);
            if (body.Position.X - Metrics::MetersToPixels(0.3f) <= 0)
            {
                body.Velocity.X = Math::Abs(body.Velocity.X);
            } else if (body.Position.X + Metrics::MetersToPixels(0.3f) >= Metrics::Width)
            {
                body.Velocity.X = -Math::Abs(body.Velocity.X);
            }
            if (body.Position.Y - Metrics::MetersToPixels(0.3f) <= 0)
            {
                body.Velocity.Y = Math::Abs(body.Velocity.Y);
            } else if (body.Position.Y + Metrics::MetersToPixels(0.3f) >= Metrics::Height)
            {
                body.Velocity.Y = -Math::Abs(body.Velocity.Y);
            }

            AllBodyData[i].Shape = std::get<Math::CircleF>(World.GetCollider(_colRefs[i]).Shape) + body.Position;
        }

        for (int i = 0; i < _colRefs.size(); ++i)
        {
            if (triggerNbrPerCollider[i] > 0)
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