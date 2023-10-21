#pragma once

#include "Sample.h"
#include "Random.h"

class Forms : public Sample
{
public:
    void SetUp() override
    {
        Sample::SetUp();

        auto polRef = World.CreateBody();
        auto &pol = World.GetBody(polRef);
        pol.Position = {static_cast<float>(Metrics::Width) / 2, static_cast<float>(Metrics::Height) / 2};

        BodyRefs.push_back(polRef);

        BodyData bd;
        bd.Shape.Type = Math::ShapeType::Polygon;

        std::vector<Math::Vec2F> Vertices{{Math::Random::Range(pol.Position.X, Metrics::Width - 100.f),
                                                  Math::Random::Range(100.f, Metrics::Height - 100.f)},
                                          {Math::Random::Range(pol.Position.X, Metrics::Width - 100.f),
                                                  Math::Random::Range(100.f, Metrics::Height - 100.f)},
                                          {Math::Random::Range(pol.Position.X, Metrics::Width - 100.f),
                                                  Math::Random::Range(100.f, Metrics::Height - 100.f)},
                                          {Math::Random::Range(pol.Position.X, Metrics::Width - 100.f),
                                                  Math::Random::Range(100.f, Metrics::Height - 100.f)},
                                          {Math::Random::Range(pol.Position.X, Metrics::Width - 100.f),
                                                  Math::Random::Range(100.f, Metrics::Height - 100.f)},
                                          {Math::Random::Range(pol.Position.X, Metrics::Width - 100.f),
                                                  Math::Random::Range(100.f, Metrics::Height - 100.f)},
                                          {Math::Random::Range(pol.Position.X, Metrics::Width - 100.f),
                                                  Math::Random::Range(100.f, Metrics::Height - 100.f)},
                                          {Math::Random::Range(pol.Position.X, Metrics::Width - 100.f),
                                                  Math::Random::Range(100.f, Metrics::Height - 100.f)}};

        bd.Shape.Polygon = new Math::Polygon(Vertices);
        bd.Color = {0, 255, 0, 255};
        AllBodyData.push_back(bd);

        auto rectRef = World.CreateBody();
        auto &rect = World.GetBody(rectRef);
        rect.Position = {static_cast<float>(Metrics::Width) / 2, static_cast<float>(Metrics::Height) / 2};

        BodyRefs.push_back(rectRef);
        BodyData rbd;
        rbd.Shape.Type = Math::ShapeType::Rectangle;
        rbd.Shape.Rectangle = new Math::Rectangle(Math::Vec2F(15.f, 100.f),Math::Vec2F(200.f, 500.f));
        rbd.Color = {0, 255, 0, 255};
        AllBodyData.push_back(rbd);
    }
};