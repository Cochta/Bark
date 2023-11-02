#include "StarSystemSample.h"

void StarSystemSample::SampleSetUp() noexcept
{
    {
        _sunRef = _world.CreateBody();
        auto &sun = _world.GetBody(_sunRef);
        sun.Position = {static_cast<float>(Metrics::Width) / 2, static_cast<float>(Metrics::Height) / 2};
        sun.Mass = 1000000;

        _bodyRefs.push_back(_sunRef);
        GraphicsData sbd;
        _circles.emplace_back(Math::Vec2F::Zero(), Metrics::MetersToPixels(0.03f));
        sbd.Color = {255, 255, 0, 255};
        AllGraphicsData.push_back(sbd);

        for (std::size_t i = 0; i < PLANET_NBR; ++i)
        {
            // Engine
            auto bodyRef = _world.CreateBody();
            auto &body = _world.GetBody(bodyRef);
            body.Position = {Math::Random::Range(100.f, Metrics::Width - 100.f),
                             Math::Random::Range(100.f, Metrics::Height - 100.f)};
            auto r = _world.GetBody(_sunRef).Position - body.Position;
            auto v = sqrt(G * (_world.GetBody(_sunRef).Mass / r.Length()));
            body.Velocity = Math::Vec2F(-r.Y, r.X).Normalized() * v;
            body.Mass = 10.f;

            // Graphics
            _bodyRefs.push_back(bodyRef);
            GraphicsData pbd;
            _circles.emplace_back(Math::Vec2F::Zero(),
                                  Math::Random::Range(
                                          Metrics::MetersToPixels(0.05f),
                                          Metrics::MetersToPixels(0.15f)));
            pbd.Color = {
                    Math::Random::Range(0, 255),
                    Math::Random::Range(0, 255),
                    Math::Random::Range(0, 255),
                    255};

            AllGraphicsData.push_back(pbd);
        }
    }
}

void StarSystemSample::CalculateGravitationalForce(const Body &sun, Body &body) noexcept
{

    auto m1m2 = sun.Mass * body.Mass;
    auto r = (sun.Position - body.Position).Length();
    auto r2 = r * r;
    auto F = G * (m1m2 / r2);

    Math::Vec2F forceDirection = (sun.Position - body.Position).Normalized();
    Math::Vec2F force = forceDirection * F;
    body.ApplyForce(force);

}

void StarSystemSample::SampleUpdate() noexcept
{
    auto &sun = _world.GetBody(_sunRef);

    for (std::size_t i = 0; i < _bodyRefs.size(); ++i)
    {
        auto &body = _world.GetBody(_bodyRefs[i]);

        AllGraphicsData[i].Shape = _circles[i] + body.Position;

        if (_bodyRefs[i] == _sunRef) continue; // Skip the Sun

        CalculateGravitationalForce(sun, body);

    }
}

void StarSystemSample::SampleTearDown() noexcept
{
    _circles.clear();
}

