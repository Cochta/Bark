#include "FormsTriggerSample.h"

void FormsTriggerSample::BeginContact(ColliderRef &col1, ColliderRef &col2) noexcept
{
    _triggerNbrPerCollider[col1.Index]++;
    _triggerNbrPerCollider[col2.Index]++;
}

void FormsTriggerSample::EndContact(ColliderRef &col1, ColliderRef &col2) noexcept
{
    _triggerNbrPerCollider[col1.Index]--;
    _triggerNbrPerCollider[col2.Index]--;
}

void FormsTriggerSample::SampleSetUp() noexcept
{
    World.SetContactListener(this);

    _triggerNbrPerCollider.resize(3, 0);

    auto polRef = World.CreateBody();
    auto &pol = World.GetBody(polRef);
    pol.Position = {static_cast<float>(Metrics::Width) / 2, static_cast<float>(Metrics::Height) / 2};
    pol.Mass = 4.f;
    _bodyRefs.push_back(polRef);

    auto colRefPol = World.CreateCollider(polRef);
    _colRefs.push_back(colRefPol);
    auto &colPol = World.GetCollider(colRefPol);

    std::vector<Math::Vec2F> vertices = {{0.f,   0.f},
                                         {200.f, 330.f},
                                         {350.f, 400.f}};

    colPol.Shape = Math::Polygon(vertices);

    BodyData bdPol;
    bdPol.Shape = Math::Polygon(vertices) + pol.Position;
    AllBodyData.push_back(bdPol);

    auto rectRef = World.CreateBody();
    auto &rect = World.GetBody(rectRef);
    rect.Position = Math::Vec2F::Zero();

    _bodyRefs.push_back(rectRef);

    auto colRefRect = World.CreateCollider(rectRef);
    _colRefs.push_back(colRefRect);
    auto &colRect = World.GetCollider(colRefRect);

    colRect.Shape = Math::Rectangle(Math::Vec2F::Zero(), Math::Vec2F(200.f, 500.f));

    BodyData rbd;
    rbd.Shape = Math::Rectangle(Math::Vec2F::Zero(), Math::Vec2F(200.f, 500.f)) + rect.Position;
    AllBodyData.push_back(rbd);


    _mouseBodyRef = World.CreateBody();
    _bodyRefs.push_back(_mouseBodyRef);
    auto &body1 = World.GetBody(_mouseBodyRef);
    body1.Position = {200.f, 700.f};
    auto mouseColRef = World.CreateCollider(_mouseBodyRef);
    _colRefs.push_back(mouseColRef);
    auto &mouseCol = World.GetCollider(mouseColRef);
    mouseCol.Shape = Math::Circle(Math::Vec2F::Zero(), Metrics::MetersToPixels(0.3f));

    BodyData bd;
    bd.Shape = mouseCol.Shape;
    AllBodyData.push_back(bd);
}

void FormsTriggerSample::SampleUpdate() noexcept
{
    auto &mouseBody = World.GetBody(_mouseBodyRef);
    mouseBody.Position = _mousePos;

    AllBodyData[2].Shape = std::get<Math::CircleF>(World.GetCollider(_colRefs[2]).Shape) + mouseBody.Position;

    for (int i = 0; i < _colRefs.size(); ++i)
    {
        if (_triggerNbrPerCollider[i] > 0)
        {
            AllBodyData[i].Color = {0, 255, 0, 255};
        } else
        {
            AllBodyData[i].Color = {0, 0, 255, 255};
        }
    }
}