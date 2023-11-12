#include "QuadTree.h"

void QuadNode::Subdivide() noexcept
{
	const Math::Vec2F halfSize = (Bounds.MaxBound() - Bounds.MinBound()) / 2;
	const Math::Vec2F minBound = Bounds.MinBound();

	Children[0] = std::make_unique<QuadNode>(QuadNode({ minBound, minBound + halfSize }, Alloc));
	Children[1] = std::make_unique<QuadNode>(QuadNode({ {minBound.X,              minBound.Y + halfSize.Y},
								{minBound.X + halfSize.X, minBound.Y + 2 * halfSize.Y} }, Alloc));
	Children[2] = std::make_unique<QuadNode>(QuadNode({ {minBound.X + halfSize.X,     minBound.Y},
								{minBound.X + 2 * halfSize.X, minBound.Y + halfSize.Y} }, Alloc));
	Children[3] = std::make_unique<QuadNode>(QuadNode(
		{ {minBound.X + halfSize.X, minBound.Y + halfSize.Y}, Bounds.MaxBound() }, Alloc));

	for (const auto& child : Children)
	{
		child->_depth = _depth + 1;
	}
}

void QuadNode::Insert(const ColliderRefAabb& colliderRefAabb) noexcept
{
	if (Children[0] != nullptr)
	{
		for (const auto& child : Children)
		{
			if (Math::Intersect(colliderRefAabb.Aabb, child->Bounds))
			{
				child->Insert(colliderRefAabb);
			}
		}
	}
	else if (ColliderRefAabbs.size() >= _MAX_COL_NBR && _depth < _MAX_DEPTH)
	{
		Subdivide();
		ColliderRefAabbs.push_back(colliderRefAabb);
		for (const auto& child : Children)
		{
			for (auto& col : ColliderRefAabbs)
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

QuadNode::QuadNode(Allocator& alloc) noexcept : ColliderRefAabbs(StandardAllocator<ColliderRefPair>{alloc}), Alloc(alloc)
{
	ColliderRefAabbs.reserve(16);
}

QuadNode::QuadNode(const Math::RectangleF& bounds, Allocator& alloc) noexcept : ColliderRefAabbs(StandardAllocator<ColliderRefPair>{ alloc }), Bounds(bounds), Alloc(alloc)
{
	ColliderRefAabbs.reserve(16);
}

void QuadNode::SetUpRoot(const Math::RectangleF& bounds) noexcept
{
#ifdef TRACY_ENABLE
	ZoneScoped;
#endif
	ColliderRefAabbs.clear();

	for (auto& child : Children)
	{
		child = nullptr;
	}
	Bounds = bounds;
}

//QuadTree::QuadTree()
//{
//	std::size_t result = 0;
//	for (size_t i = 0; i < _MAX_DEPTH; i++)
//	{
//		result += Math::Pow(4, i);
//	}
//	Nodes.resize(result);
//}
