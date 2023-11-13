#include "QuadTree.h"

QuadNode::QuadNode(Allocator& alloc) noexcept : ColliderRefAabbs(StandardAllocator<ColliderRefPair>{alloc})
{
	ColliderRefAabbs.reserve(16);
}

QuadNode::QuadNode(const Math::RectangleF& bounds, Allocator& alloc) noexcept : ColliderRefAabbs(StandardAllocator<ColliderRefPair>{ alloc }), Bounds(bounds)
{
	ColliderRefAabbs.reserve(16);
}

QuadTree::QuadTree(Allocator& alloc) noexcept : Alloc(alloc), Nodes{ StandardAllocator<QuadNode>{alloc} }
{
	std::size_t result = 0;
	for (size_t i = 0; i <= _MAX_DEPTH; i++)
	{
		result += Math::Pow(4, i);
	}
	Nodes.resize(result, QuadNode(Alloc));
}

void QuadTree::SubdivideNode(QuadNode& node) noexcept
{
	const Math::Vec2F halfSize = (node.Bounds.MaxBound() - node.Bounds.MinBound()) / 2;
	const Math::Vec2F minBound = node.Bounds.MinBound();

	node.Children[0] = &Nodes[NodeIndex];
	node.Children[0]->Bounds = { minBound, minBound + halfSize };

	node.Children[1] = &Nodes[NodeIndex + 1];
	node.Children[1]->Bounds = { {minBound.X, minBound.Y + halfSize.Y},{minBound.X + halfSize.X, minBound.Y + 2 * halfSize.Y} };

	node.Children[2] = &Nodes[NodeIndex + 2];
	node.Children[2]->Bounds = { {minBound.X + halfSize.X, minBound.Y},{minBound.X + 2 * halfSize.X, minBound.Y + halfSize.Y} };

	node.Children[3] = &Nodes[NodeIndex + 3];
	node.Children[3]->Bounds = { {minBound.X + halfSize.X, minBound.Y + halfSize.Y}, node.Bounds.MaxBound() };


	for (const auto& child : node.Children)
	{
		child->Depth = node.Depth + 1;
	}
	NodeIndex += 4;
}

void QuadTree::Insert(QuadNode& node, const ColliderRefAabb& colliderRefAabb) noexcept
{
	if (node.Children[0] != nullptr)
	{
		for (const auto& child : node.Children)
		{
			if (Math::Intersect(colliderRefAabb.Aabb, child->Bounds))
			{
				Insert(*child, colliderRefAabb);
			}
		}
	}
	else if (node.ColliderRefAabbs.size() >= _MAX_COL_NBR && node.Depth < _MAX_DEPTH)
	{
		SubdivideNode(node);
		node.ColliderRefAabbs.push_back(colliderRefAabb);
		for (const auto& child : node.Children)
		{
			for (auto& col : node.ColliderRefAabbs)
			{
				if (Math::Intersect(col.Aabb, child->Bounds))
				{
					Insert(*child, col);
				}
			}
		}
		node.ColliderRefAabbs.clear();
	}
	else
	{
		node.ColliderRefAabbs.push_back(colliderRefAabb);
	}
}

void QuadTree::SetUpRoot(const Math::RectangleF& bounds) noexcept
{
#ifdef TRACY_ENABLE
	ZoneScoped;
#endif
	for (auto& node : Nodes)
	{
		node.ColliderRefAabbs.clear();
		std::fill(node.Children.begin(), node.Children.end(), nullptr);
	}
	Nodes[0].Bounds = bounds;

	NodeIndex = 1;
}
