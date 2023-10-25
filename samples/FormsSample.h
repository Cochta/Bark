#pragma once

#include "Sample.h"
#include "Random.h"

class FormsSample : public Sample {
public:

    BodyRef mouseBodyRef;
    std::vector<Math::Vec2F> Vertices;

    void SetUp() override {
        Sample::SetUp();

        //World.SetContactListener(new TriggerListener);// chan^gger

        auto polRef = World.CreateBody();
        auto &pol = World.GetBody(polRef);
        pol.Position = {static_cast<float>(Metrics::Width) / 2, static_cast<float>(Metrics::Height) / 2};
        pol.Mass = 4.f;
        BodyRefs.push_back(polRef);

        auto colRefPol = World.CreateCollider(polRef);
        ColRefs.push_back(colRefPol);
        auto &colPol = World.GetCollider(colRefPol);

        Vertices = {{Math::Random::Range(pol.Position.X, Metrics::Width - 100.f),
                            Math::Random::Range(100.f, Metrics::Height - 100.f)},
                    {Math::Random::Range(pol.Position.X, Metrics::Width - 100.f),
                            Math::Random::Range(100.f, Metrics::Height - 100.f)},
                    {Math::Random::Range(pol.Position.X, Metrics::Width - 100.f),
                            Math::Random::Range(100.f, Metrics::Height - 100.f)}};

        colPol.Shape = Math::Polygon(Vertices);

        BodyData bdPol;
        bdPol.Shape = colPol.Shape;
        AllBodyData.push_back(bdPol);

        auto rectRef = World.CreateBody();
        auto &rect = World.GetBody(rectRef);
        rect.Position = Math::Vec2F::Zero();

        BodyRefs.push_back(rectRef);

        auto colRefRect = World.CreateCollider(rectRef);
        ColRefs.push_back(colRefRect);
        auto &colRect = World.GetCollider(colRefRect);

        colRect.Shape = Math::Rectangle(Math::Vec2F::Zero(), Math::Vec2F(200.f, 500.f));

        BodyData rbd;
        rbd.Shape = colRect.Shape;
        AllBodyData.push_back(rbd);


        mouseBodyRef = World.CreateBody();
        BodyRefs.push_back(mouseBodyRef);
        auto &body1 = World.GetBody(mouseBodyRef);
        body1.Position = {200.f, 700.f};
        auto mouseColRef = World.CreateCollider(mouseBodyRef);
        ColRefs.push_back(mouseColRef);
        auto &mouseCol = World.GetCollider(mouseColRef);
        mouseCol.Shape = Math::Circle(Math::Vec2F::Zero(), Metrics::MetersToPixels(0.3f));

        BodyData bd;
        bd.Shape = mouseCol.Shape;
        AllBodyData.push_back(bd);
    }

    void Update() override {
        //Vertices[0] = _mousePos;
        //World.GetCollider(ColRefs[0]).ColShape->Polygon->SetVertices(Vertices);
        World.GetBody(mouseBodyRef).Position = _mousePos;
        for (int i = 0; i < ColRefs.size(); ++i) {
//            if (World.GetCollider(ColRefs[i]).DoesTrigger())
//            {
//                AllBodyData[i].Color = {0, 255, 0, 255};
//            } else
//            {
//                AllBodyData[i].Color = {0, 0, 255, 255};
//            }
        }
        Sample::Update();
    }
};