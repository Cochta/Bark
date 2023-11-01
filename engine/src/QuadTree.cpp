#include "QuadTree.h"

void QuadNode::Subdivide() noexcept // pas besoin de mettre depth dans le neud
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

void QuadNode::Insert( const ColliderRefAabb &colliderRefAabb) noexcept
{
    if (Children[0] != nullptr)
    {
        for (auto &child: Children)
        {
            if (Math::Intersect(colliderRefAabb.Aabb, child->Bounds))
            {
                child->Insert(colliderRefAabb);
            }
        }
    }
    else if (ColliderRefAabbs.size() >= MaxColNbr && _depth < _maxDepth)
    {
        Subdivide();
        ColliderRefAabbs.push_back(colliderRefAabb);
        for (auto &child: Children)
        {
            for (auto &col: ColliderRefAabbs)
            {
                if (Math::Intersect(col.Aabb, child->Bounds))
                {
                    child->Insert(col);
                }
            }
        }
        ColliderRefAabbs.clear();
    }
    else
    {
        ColliderRefAabbs.push_back(colliderRefAabb);
    }
}

void QuadNode::Delete()
{
    if (Children[0] != nullptr)
    {
        for (auto &child: Children)
        {
            child->Delete();
        }
    }
    for (auto &child: Children)
    {
        delete child;
        child = nullptr;
    }
}

void QuadTree::SetUp(const Math::RectangleF &bounds) noexcept
{
    TearDown();
    _root.Children = {nullptr, nullptr, nullptr, nullptr};
    _root.Bounds = bounds;
}

void QuadTree::TearDown() noexcept
{
    _root.Delete();
}
