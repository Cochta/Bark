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
    _world.SetContactListener(this);

    _triggerNbrPerCollider.resize(3, 0);

    auto polRef = _world.CreateBody();
    auto &pol = _world.GetBody(polRef);
    pol.Position = {static_cast<float>(Metrics::Width) / 2, static_cast<float>(Metrics::Height) / 2};
    pol.Mass = 4.f;
    _bodyRefs.push_back(polRef);

    auto colRefPol = _world.CreateCollider(polRef);
    _colRefs.push_back(colRefPol);
    auto &colPol = _world.GetCollider(colRefPol);

    std::vector<Math::Vec2F> vertices = {{0.f,   0.f},
                                         {200.f, 330.f},
                                         {350.f, 400.f}};

    colPol.Shape = Math::Polygon(vertices);

    GraphicsData bdPol;
    bdPol.Shape = Math::Polygon(vertices) + pol.Position;
    AllGraphicsData.push_back(bdPol);

    auto rectRef = _world.CreateBody();
    auto &rect = _world.GetBody(rectRef);
    rect.Position = Math::Vec2F::Zero();

    _bodyRefs.push_back(rectRef);

    auto colRefRect = _world.CreateCollider(rectRef);
    _colRefs.push_back(colRefRect);
    auto &colRect = _world.GetCollider(colRefRect);

    colRect.Shape = Math::Rectangle(Math::Vec2F::Zero(), Math::Vec2F(200.f, 500.f));

    GraphicsData rbd;
    rbd.Shape = Math::Rectangle(Math::Vec2F::Zero(), Math::Vec2F(200.f, 500.f)) + rect.Position;
    AllGraphicsData.push_back(rbd);


    _mouseBodyRef = _world.CreateBody();
    _bodyRefs.push_back(_mouseBodyRef);
    auto &body1 = _world.GetBody(_mouseBodyRef);
    body1.Position = {200.f, 700.f};
    auto mouseColRef = _world.CreateCollider(_mouseBodyRef);
    _colRefs.push_back(mouseColRef);
    auto &mouseCol = _world.GetCollider(mouseColRef);
    mouseCol.Shape = Math::Circle(Math::Vec2F::Zero(), Metrics::MetersToPixels(0.3f));

    GraphicsData bd;
    bd.Shape = mouseCol.Shape;
    AllGraphicsData.push_back(bd);
}

void FormsTriggerSample::SampleUpdate() noexcept
{
    auto &mouseBody = _world.GetBody(_mouseBodyRef);
    mouseBody.Position = _mousePos;

    AllGraphicsData[2].Shape = std::get<Math::CircleF>(_world.GetCollider(_colRefs[2]).Shape) + mouseBody.Position;

    for (int i = 0; i < _colRefs.size(); ++i)
    {
        if (_triggerNbrPerCollider[i] > 0)
        {
            AllGraphicsData[i].Color = {0, 255, 0, 255};
        } else
        {
            AllGraphicsData[i].Color = {0, 0, 255, 255};
        }
    }
}

void FormsTriggerSample::SampleTearDown() noexcept
{
    _triggerNbrPerCollider.clear();
}
