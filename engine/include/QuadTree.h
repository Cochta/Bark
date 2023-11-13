#pragma once

#include "Collider.h"
#include "Allocators.h"
#include "UniquePtr.h"

#include <memory>

struct ColliderRefAabb
{
	const Math::RectangleF Aabb;
	const ColliderRef ColRef;
};

class QuadNode
{
public:
	Math::RectangleF Bounds{ Math::Vec2F::Zero(), Math::Vec2F::Zero() };

	std::array<QuadNode*, 4> Children{ nullptr, nullptr, nullptr, nullptr };

	CustomlyAllocatedVector<ColliderRefAabb> ColliderRefAabbs;

	int Depth = 0;

public:
	QuadNode(Allocator& alloc) noexcept;

	explicit QuadNode(const Math::RectangleF& bounds, Allocator& alloc) noexcept;

};

class QuadTree {
public:
	CustomlyAllocatedVector<QuadNode> Nodes;

	static constexpr int _MAX_COL_NBR = 16;

	static constexpr int _MAX_DEPTH = 5;

	int NodeIndex = 1;

	Allocator& Alloc;

	QuadTree(Allocator& alloc) noexcept;

	void SetUpRoot(const Math::RectangleF& bounds) noexcept;

	void Insert(QuadNode& node, const ColliderRefAabb& colliderRefAabb) noexcept;

	void SubdivideNode(QuadNode& node) noexcept;
};

