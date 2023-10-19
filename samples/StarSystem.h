//
// Created by const on 18/10/2023.
//
#pragma once

#include "Sample.h"
#include "Random.h"

class StarSystem : public Sample
{
public:

    static constexpr float G = 6.67f;
    static constexpr std::size_t planetNbr = 50000;

    void SetUp() override
    {
        Sample::SetUp();
        auto sunRef = World.CreateBody();
        auto &sun = World.GetBody(sunRef);
        sun.Position = {static_cast<float>(Metrics::Width) / 2, static_cast<float>(Metrics::Height) / 2};
        sun.Mass = 1000000;

        BodyRefs.push_back(sunRef);
        BodyData sbd{BodyType::Sun,
                     Metrics::MetersToPixels(0.03),
                     {255, 255, 0, 255}};
        AllBodyData.push_back(sbd);

        const auto copiedSun = sun;

        for (std::size_t i = 0; i < planetNbr; ++i)
        {
            // Engine
            auto bodyRef = World.CreateBody();
            auto &body = World.GetBody(bodyRef);
            body.Position = {Random::Range(100.f, Metrics::Width - 100.f),
                             Random::Range(100.f, Metrics::Height - 100.f)};
            auto r = copiedSun.Position - body.Position;
            auto v = sqrt(G * (copiedSun.Mass / r.Length()));
            body.Velocity = Vec2F(-r.Y, r.X).Normalized() * v;
            body.Mass = 10.f;

            // Graphics
            BodyRefs.push_back(bodyRef);
            BodyData pbd{BodyType::Planet,
                         Random::Range(
                                 Metrics::MetersToPixels(0.05f),
                                 Metrics::MetersToPixels(0.15f)),
                         {Random::Range(0, 255),
                          Random::Range(0, 255),
                          Random::Range(0, 255), 255}};
            AllBodyData.push_back(pbd);
        }
    }

    static void CalculateGravitationalForce(const Body &sun, Body &body)
    {
        auto m1m2 = sun.Mass * body.Mass;
        auto r = (sun.Position - body.Position).Length();
        auto r2 = r * r;
        auto F = G * (m1m2 / r2);

        Vec2F forceDirection = (sun.Position - body.Position).Normalized();
        Vec2F force = forceDirection * F;
        body.ApplyForce(force);
    }

    BodyRef FindSunRef()
    {
        auto itSun = std::find_if(AllBodyData.begin(), AllBodyData.end(),
                                  [](const BodyData &bd)
                                  { return bd.Type == BodyType::Sun; });

        auto idx = std::distance(AllBodyData.begin(), itSun);
        return BodyRefs[idx];
    }

    void Update() override
    {
        auto sunRef = FindSunRef();

        auto &sun = World.GetBody(sunRef);

        for (auto &bodyRef: BodyRefs)
        {
            if (bodyRef == sunRef)
            {
                continue; // Skip the Sun
            }

            auto &body = World.GetBody(bodyRef);
            CalculateGravitationalForce(sun, body);
        }
        Sample::Update();
    }
};
