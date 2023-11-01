#pragma once

#include "Collider.h"


class QuadNode
{
public:
    Math::RectangleF Bounds{Math::Vec2F::Zero(), Math::Vec2F::Zero()};
    std::array<QuadNode *, 4> Children{nullptr, nullptr, nullptr, nullptr};
    std::vector<std::pair<Collider&, Math::Vec2F>> Colliders;

    QuadNode() noexcept = default;

    QuadNode(Math::RectangleF bounds) noexcept: Bounds(bounds)
    {};

    void Subdivide() noexcept;

    void Insert(std::pair<Collider&, Math::Vec2F> collider) noexcept;

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