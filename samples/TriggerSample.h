//
// Created by Coch on 20.10.2023.
//

#pragma once

#include <iostream>
#include "Sample.h"
#include "Random.h"


class TriggerSample : public Sample, public ContactListener {// todo: faire l héritage du listener
    static constexpr std::size_t CIRCLE_NBR = 100;
    static constexpr float SPEED = 100;

    void BeginContact(ColliderRef &col1, ColliderRef &col2) noexcept override {// todo: not out param
        AllBodyData[col1.Index].Color =  {0, 255, 0, 255};
        AllBodyData[col2.Index].Color =  {0, 255, 0, 255};
    }

    void EndContact(ColliderRef &col1, ColliderRef &col2) noexcept override {// todo: not out param
        AllBodyData[col1.Index].Color =  {0, 0, 255, 255};
        AllBodyData[col2.Index].Color =  {0, 0, 255, 255};
    }


    void SetUp() override {
        Sample::SetUp();

        World.SetContactListener(this);


        for (std::size_t i = 0; i < CIRCLE_NBR; ++i) {
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
            col1.Shape = Math::Circle(Math::Vec2F::Zero(), Metrics::MetersToPixels(0.3f));

            BodyData bd;
            bd.Shape = col1.Shape;
            bd.Color = {0, 0, 255, 255};
            AllBodyData.push_back(bd);

        }
    }

    void Update() override {
        for (auto bodyRef: BodyRefs) {
            auto &body = World.GetBody(bodyRef);
            if (body.Position.X - Metrics::MetersToPixels(0.3f) <= 0) {
                body.Velocity.X = Math::Abs(body.Velocity.X);
            } else if (body.Position.X + Metrics::MetersToPixels(0.3f) >= Metrics::Width) {
                body.Velocity.X = -Math::Abs(body.Velocity.X);
            }
            if (body.Position.Y - Metrics::MetersToPixels(0.3f) <= 0) {
                body.Velocity.Y = Math::Abs(body.Velocity.Y);
            } else if (body.Position.Y + Metrics::MetersToPixels(0.3f) >= Metrics::Height) {
                body.Velocity.Y = -Math::Abs(body.Velocity.Y);
            }
        }

//        for (int i = 0; i < ColRefs.size(); ++i) {
//            if (World.GetCollider(ColRefs[i]).DoesTrigger()) {
//                AllBodyData[i].Color = {0, 255, 0, 255};
//            } else {
//                AllBodyData[i].Color = {0, 0, 255, 255};
//            }
//        }
        Sample::Update();
    }
};