#pragma once

#include "Sample.h"
#include "Random.h"

class StarSystem : public Sample
{
public:

    static constexpr float G = 6.67f;
    static constexpr std::size_t PLANET_NBR = 1000;
    BodyRef SUN_REF;

    void SetUp() override
    {
        Sample::SetUp();
        auto sunRef = World.CreateBody();
        auto &sun = World.GetBody(sunRef);
        sun.Position = {static_cast<float>(Metrics::Width) / 2, static_cast<float>(Metrics::Height) / 2};
        sun.Mass = 1000000;

        BodyRefs.push_back(sunRef);
        SUN_REF = sunRef;
        BodyData sbd;
        sbd.Shape.Type = Math::ShapeType::Circle;
        sbd.Shape.Circle = new Math::Circle(Math::Vec2F::Zero(), Metrics::MetersToPixels(0.03));
        sbd.Color = {255, 255, 0, 255};
        AllBodyData.push_back(sbd);

        const auto copiedSun = sun;

        for (std::size_t i = 0; i < PLANET_NBR; ++i)
        {
            // Engine
            auto bodyRef = World.CreateBody();
            auto &body = World.GetBody(bodyRef);
            body.Position = {Math::Random::Range(100.f, Metrics::Width - 100.f),
                             Math::Random::Range(100.f, Metrics::Height - 100.f)};
            auto r = copiedSun.Position - body.Position;
            auto v = sqrt(G * (copiedSun.Mass / r.Length()));
            body.Velocity = Math::Vec2F(-r.Y, r.X).Normalized() * v;
            body.Mass = 10.f;

            // Graphics
            BodyRefs.push_back(bodyRef);
            BodyData pbd;
            pbd.Shape.Type = Math::ShapeType::Circle;
            pbd.Shape.Circle = new Math::Circle(Math::Vec2F::Zero(),
                                                Math::Random::Range(
                                                        Metrics::MetersToPixels(0.05f),
                                                        Metrics::MetersToPixels(0.15f)));
            pbd.Color = {
                    Math::Random::Range(0, 255),
                    Math::Random::Range(0, 255),
                    Math::Random::Range(0, 255),
                    255};

            AllBodyData.push_back(pbd);
        }
    }

    static

    void CalculateGravitationalForce(const Body &sun, Body &body)
    {
        auto m1m2 = sun.Mass * body.Mass;
        auto r = (sun.Position - body.Position).Length();
        auto r2 = r * r;
        auto F = G * (m1m2 / r2);

        Math::Vec2F forceDirection = (sun.Position - body.Position).Normalized();
        Math::Vec2F force = forceDirection * F;
        body.ApplyForce(force);
    }

    void Update()

    override
    {
        auto &sun = World.GetBody(SUN_REF);

        for (auto BodyRef : BodyRefs)
        {
            if (BodyRef == SUN_REF) continue; // Skip the Sun

            auto &body = World.GetBody(BodyRef);
            CalculateGravitationalForce(sun, body);
        }

        Sample::Update();
    }
};
