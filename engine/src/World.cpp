//
// Created by const on 13/10/2023.
//
#include "World.h"

void World::SetUp() noexcept
{
    int initSize = 100;

    _bodies.resize(initSize, Body());
    BodyGenIndices.resize(initSize, 0);
    _colliders.resize(initSize, Collider());
    ColliderGenIndices.resize(initSize, 0);
}
void World::TearDown() noexcept
{
    _bodies.clear();
    BodyGenIndices.clear();
    _colliders.clear();
    ColliderGenIndices.clear();

    _colliderIdCount = 0;

    _colPairs.clear();
}

void World::Update(float deltaTime) noexcept
{
    for (auto &body: _bodies)
    {
        if (!body.IsEnabled()) continue;
        auto acceleration = body.GetForce() / body.Mass;
        body.Velocity += acceleration * deltaTime;
        body.Position += body.Velocity * deltaTime;

        body.ResetForce();

    }
    for (auto &col1: _colliders)
    {
        if (!col1.IsAttached) continue;
        switch (col1.ColShape->Type)
        {
            case Math::ShapeType::Circle:
                col1.ColShape->Circle->SetCenter(GetBody(col1.BodyRef).Position);
                break;
        }
        //col1.ColShape->Polygon->SetVertices(col1.BodyRef)

        if (!col1.IsSensor) continue;

        for (auto &col2: _colliders)
        {
            if (col1.BodyRef == col2.BodyRef) continue;
            if (!col2.IsAttached) continue;
            if (!col1.IsSensor) continue;

            if (_colPairs.find({col1, col2}) != _colPairs.end())
            {
                if (!Overlap(col1, col2))
                {
                    col1.OnTriggerExit();
                    col2.OnTriggerExit();
                    _colPairs.erase({col1, col2});
                }
                continue;
            }

            if (Overlap(col1, col2))
            {
                col1.OnTriggerEnter();
                col2.OnTriggerEnter();
                _colPairs.insert({col1, col2});
            }
        }
    }
}

[[nodiscard]] BodyRef World::CreateBody() noexcept
{
    auto it = std::find_if(_bodies.begin(), _bodies.end(), [](const Body &body)
    {
        return !body.IsEnabled(); // Get first Disabled body
    });

    if (it != _bodies.end())
    {
        std::size_t index = std::distance(_bodies.begin(), it);
        auto bodyRef = BodyRef{index, BodyGenIndices[index]};
        GetBody(bodyRef).Enable();
        return bodyRef;
    }

    std::size_t previousSize = _bodies.size();

    _bodies.resize(previousSize * 2, Body());
    BodyGenIndices.resize(previousSize * 2, 0);

    BodyRef bodyRef = {previousSize, BodyGenIndices[previousSize]};
    GetBody(bodyRef).Enable();
    return bodyRef;
}

void World::DestroyBody(BodyRef bodyRef)
{
    if (BodyGenIndices[bodyRef.Index] != bodyRef.GenIndex)
    {
        throw std::runtime_error("No body found !");
    }

    _bodies[bodyRef.Index].Disable();
}

[[nodiscard]] Body &World::GetBody(BodyRef bodyRef)
{
    if (BodyGenIndices[bodyRef.Index] != bodyRef.GenIndex)
    {
        throw std::runtime_error("No body found !");
    }

    return _bodies[bodyRef.Index];
}

ColliderRef World::CreateCollider(BodyRef bodyRef) noexcept
{
    _colliderIdCount++;
    auto it = std::find_if(_colliders.begin(), _colliders.end(), [](const Collider &collider)
    {
        return !collider.IsAttached; // Get first Disabled collider
    });

    if (it != _colliders.end())
    {
        std::size_t index = std::distance(_colliders.begin(), it);
        auto colRef = ColliderRef{index, ColliderGenIndices[index]};
        auto &col = GetCollider(colRef);
        col.IsAttached = true;
        col.BodyRef = bodyRef;
        col.ID = _colliderIdCount;

        return colRef;
    }

    std::size_t previousSize = _colliders.size();

    _colliders.resize(previousSize * 2, Collider());
    ColliderGenIndices.resize(previousSize * 2, 0);

    ColliderRef colRef = {previousSize, ColliderGenIndices[previousSize]};
    auto &col = GetCollider(colRef);
    col.IsAttached = true;
    col.BodyRef = bodyRef;
    col.ID = _colliderIdCount;
    return colRef;
}

Collider &World::GetCollider(ColliderRef colRef)
{
    if (ColliderGenIndices[colRef.Index] != colRef.GenIndex)
    {
        throw std::runtime_error("No collider found !");
    }

    return _colliders[colRef.Index];
}

void World::DestroyCollider(ColliderRef colRef)
{
    if (ColliderGenIndices[colRef.Index] != colRef.GenIndex)
    {
        throw std::runtime_error("No collider found !");
    }
    _colliders[colRef.Index].IsAttached = false;
}

bool World::Overlap(const Collider &colA, const Collider &colB) const
{
    bool DoesOverlap = false;
    switch (colA.ColShape->Type)
    {
        case Math::ShapeType::Circle:
            switch (colB.ColShape->Type)
            {
                case Math::ShapeType::Circle:
                    DoesOverlap = Math::Intersect(*colA.ColShape->Circle, *colB.ColShape->Circle);
                    break;
                case Math::ShapeType::Rectangle:
                    DoesOverlap = Math::Intersect(*colA.ColShape->Circle, *colB.ColShape->Rectangle);
                    break;
                case Math::ShapeType::Polygon:
                    DoesOverlap = Math::Intersect(*colA.ColShape->Circle, *colB.ColShape->Polygon);
                    break;
            }
            break;
        case Math::ShapeType::Rectangle:
            switch (colB.ColShape->Type)
            {
                case Math::ShapeType::Circle:
                    DoesOverlap = Math::Intersect(*colA.ColShape->Rectangle, *colB.ColShape->Circle);
                    break;
                case Math::ShapeType::Rectangle:
                    DoesOverlap = Math::Intersect(*colA.ColShape->Rectangle, *colB.ColShape->Rectangle);
                    break;
                case Math::ShapeType::Polygon:
                    DoesOverlap = Math::Intersect(*colA.ColShape->Rectangle, *colB.ColShape->Polygon);
                    break;
            }
            break;
        case Math::ShapeType::Polygon:
            switch (colB.ColShape->Type)
            {
                case Math::ShapeType::Circle:
                    DoesOverlap = Math::Intersect(*colA.ColShape->Polygon, *colB.ColShape->Circle);
                    break;
                case Math::ShapeType::Rectangle:
                    DoesOverlap = Math::Intersect(*colA.ColShape->Polygon, *colB.ColShape->Rectangle);
                    break;
                case Math::ShapeType::Polygon:
                    DoesOverlap = Math::Intersect(*colA.ColShape->Polygon, *colB.ColShape->Polygon);
                    break;
            }
            break;
    }
    return DoesOverlap;
}
