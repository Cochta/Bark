#pragma once

#include "Collider.h"

struct ColliderRefAabb
{
    const ColliderRef ColRef;
    const Math::RectangleF Aabb;
};

class QuadNode
{
public:
    Math::RectangleF Bounds{Math::Vec2F::Zero(), Math::Vec2F::Zero()};
    std::array<QuadNode *, 4> Children{nullptr, nullptr, nullptr, nullptr};
    std::vector<ColliderRefAabb> ColliderRefAabbs;
    
    QuadNode() noexcept = default;
    
    explicit QuadNode(Math::RectangleF bounds) noexcept: Bounds(bounds)
    {};
    
    void Subdivide() noexcept;
    
    void Insert(const ColliderRefAabb &colliderRefAabb) noexcept;
    
    void Delete();
    
    static constexpr int MaxColNbr = 16;
    
    int _depth = 0;
    
    static constexpr int _maxDepth = 5;
};

class QuadTree
{
public:
    QuadNode _root;
    
    void SetUp(const Math::RectangleF &bounds) noexcept;
    
    void TearDown() noexcept;
};