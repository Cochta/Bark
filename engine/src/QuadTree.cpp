#include "QuadTree.h"

void QuadNode::Subdivide() noexcept
{
    Math::Vec2F halfSize = (Bounds.MaxBound() - Bounds.MinBound()) / 2;
    Math::Vec2F minBound = Bounds.MinBound();

    Children[0] = new QuadNode({minBound, minBound + halfSize});
    Children[1] = new QuadNode({{minBound.X,              minBound.Y + halfSize.Y},
                                {minBound.X + halfSize.X, minBound.Y + 2 * halfSize.Y}});
    Children[2] = new QuadNode({{minBound.X + halfSize.X,     minBound.Y},
                                {minBound.X + 2 * halfSize.X, minBound.Y + halfSize.Y}});
    Children[3] = new QuadNode(
            {{minBound.X + halfSize.X, minBound.Y + halfSize.Y}, Bounds.MaxBound()});
    for (auto &child: Children)
    {
        child->_depth = _depth + 1;
    }
}

void QuadNode::Insert(Collider& collider) noexcept
{
    if (Colliders.size() >= MaxColNbr && _depth < _maxDepth)
    {
        Subdivide();
        for (auto &col: Colliders)
        {
            for (auto &child: Children)
            {
                //if collide
                child->Insert(col);
            }
        }
        Colliders.clear();
    } else
    {
        Colliders.push_back(collider);
    }
}

void QuadTree::SetUp(const Math::RectangleF &Boundary) noexcept
{
    _root.Bounds = Boundary;
}