#include "World.h"

void World::SetUp() noexcept
{
    int initSize = 100;
    
    _bodies.resize(initSize, Body());
    BodyGenIndices.resize(initSize, 0);
    _colliders.resize(initSize, Collider());
    ColliderGenIndices.resize(initSize, 0);
    
    _quadTree.SetUp(Math::RectangleF(Math::Vec2F::Zero(), Math::Vec2F::Zero()));
}

void World::TearDown() noexcept
{
    _bodies.clear();
    BodyGenIndices.clear();
    _colliders.clear();
    ColliderGenIndices.clear();
    
    _colliderIdCount = 0;
    
    _colRefPairs.clear();
}

void World::Update(float deltaTime) noexcept
{
    for (auto &body: _bodies)
    {
        if (!body.IsEnabled())
        { continue; }
        auto acceleration = body.GetForce() / body.Mass;
        body.Velocity += acceleration * deltaTime;
        body.Position += body.Velocity * deltaTime;
        
        body.ResetForce();
        
    }
    
    if (_contactListener == nullptr)
    { return; }
    
    Math::Vec2F maxBounds(std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
    Math::Vec2F minBounds(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    for (auto &collider: _colliders)
    {
        if (!collider.IsAttached)
        { continue; }
        
        collider.BodyPosition = GetBody(collider.BodyRef).Position;
        
        auto bounds = collider.GetBounds();
        if (bounds.MinBound().X < minBounds.X)
        {
            minBounds.X = bounds.MinBound().X;
        }
        if (bounds.MinBound().Y < minBounds.Y)
        {
            minBounds.Y = bounds.MinBound().Y;
        }
        if (bounds.MaxBound().X > maxBounds.X)
        {
            maxBounds.X = bounds.MaxBound().X;
        }
        if (bounds.MaxBound().Y > maxBounds.Y)
        {
            maxBounds.Y = bounds.MaxBound().Y;
        }
        
    }
    _quadTree.SetUp(Math::RectangleF(minBounds, maxBounds));
    
    for (auto &collider: _colliders)
    {
        if (!collider.IsAttached)
        { continue; }
        
        _quadTree._root.Insert({collider, GetBody(collider.BodyRef).Position});
    }
    
    for (std::size_t i = 0; i < _colliders.size(); ++i)
    {
        ColliderRef colRef1{i, ColliderGenIndices[i]};
        auto &col1 = GetCollider(colRef1);
        
        if (!col1.IsAttached)
        { continue; }
        
        if (!col1.IsSensor)
        { continue; }
        
        for (std::size_t j = 0; j < _colliders.size(); ++j)
        {
            ColliderRef colRef2{j, ColliderGenIndices[j]};
            auto &col2 = GetCollider(colRef2);
            if (col1.BodyRef == col2.BodyRef)
            { continue; }
            if (!col2.IsAttached)
            { continue; }
            if (!col1.IsSensor)
            { continue; }
            
            if (_colRefPairs.find({colRef1, colRef2}) != _colRefPairs.end())
            {
                if (!Overlap(col1, col2))
                {
                    _contactListener->EndContact(colRef1, colRef2);
                    _colRefPairs.erase({colRef1, colRef2});
                }
                continue;
            }
            
            if (Overlap(col1, col2))
            {
                _contactListener->BeginContact(colRef1, colRef2);
                _colRefPairs.insert({colRef1, colRef2});
            }
        }
    }
}

[[nodiscard]] BodyRef World::CreateBody() noexcept
{
    auto it = std::find_if(_bodies.begin(), _bodies.end(), [](const Body &body) {
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
    auto it = std::find_if(_colliders.begin(), _colliders.end(), [](const Collider &collider) {
        return !collider.IsAttached; // Get first Disabled collider
    });
    
    if (it != _colliders.end())
    {
        std::size_t index = std::distance(_colliders.begin(), it);
        auto colRef = ColliderRef{index, ColliderGenIndices[index]};
        auto &col = GetCollider(colRef);
        col.IsAttached = true;
        col.BodyRef = bodyRef;
        
        return colRef;
    }
    
    std::size_t previousSize = _colliders.size();
    
    _colliders.resize(previousSize * 2, Collider());
    ColliderGenIndices.resize(previousSize * 2, 0);
    
    ColliderRef colRef = {previousSize, ColliderGenIndices[previousSize]};
    auto &col = GetCollider(colRef);
    col.IsAttached = true;
    col.BodyRef = bodyRef;
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

[[nodiscard]] bool World::Overlap(const Collider &colA, const Collider &colB)
{
    switch (colA.Shape.index())
    {
        case static_cast<int>( Math::ShapeType::Circle):
        {
            Math::CircleF circle = std::get<Math::CircleF>(colA.Shape) + GetBody(colA.BodyRef).Position;
            switch (colB.Shape.index())
            {
                case static_cast<int>( Math::ShapeType::Circle):
                    return Math::Intersect(circle,
                                           std::get<Math::CircleF>(colB.Shape) + GetBody(colB.BodyRef).Position);
                case static_cast<int>( Math::ShapeType::Rectangle):
                    return Math::Intersect(circle,
                                           std::get<Math::RectangleF>(colB.Shape) + GetBody(colB.BodyRef).Position);
                case static_cast<int>( Math::ShapeType::Polygon):
                    return Math::Intersect(circle,
                                           std::get<Math::PolygonF>(colB.Shape) + GetBody(colB.BodyRef).Position);
            }
            break;
        }
        case static_cast<int>( Math::ShapeType::Rectangle):
        {
            Math::RectangleF rect = std::get<Math::RectangleF>(colA.Shape) + GetBody(colA.BodyRef).Position;
            switch (colB.Shape.index())
            {
                case static_cast<int>( Math::ShapeType::Circle):
                    return Math::Intersect(rect,
                                           std::get<Math::CircleF>(colB.Shape) + GetBody(colB.BodyRef).Position);
                case static_cast<int>( Math::ShapeType::Rectangle):
                    return Math::Intersect(rect,
                                           std::get<Math::RectangleF>(colB.Shape) + GetBody(colB.BodyRef).Position);
                case static_cast<int>(Math::ShapeType::Polygon):
                    return Math::Intersect(rect,
                                           std::get<Math::PolygonF>(colB.Shape) + GetBody(colB.BodyRef).Position);
            }
            break;
        }
        case static_cast<int>( Math::ShapeType::Polygon):
        {
            Math::PolygonF pol = std::get<Math::PolygonF>(colA.Shape) + GetBody(colA.BodyRef).Position;
            switch (colB.Shape.index())
            {
                case static_cast<int>( Math::ShapeType::Circle):
                    return Math::Intersect(pol,
                                           std::get<Math::CircleF>(colB.Shape) + GetBody(colB.BodyRef).Position);
                case static_cast<int>( Math::ShapeType::Rectangle):
                    return Math::Intersect(pol,
                                           std::get<Math::RectangleF>(colB.Shape) + GetBody(colB.BodyRef).Position);
                case static_cast<int>( Math::ShapeType::Polygon):
                    return Math::Intersect(pol,
                                           std::get<Math::PolygonF>(colB.Shape) + GetBody(colB.BodyRef).Position);
            }
            break;
        }
    }
    return false;
}
