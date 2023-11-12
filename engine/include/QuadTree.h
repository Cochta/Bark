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

//class QuadTree {
//public:
//	std::vector<QuadNode> Nodes;
//	static constexpr int _MAX_COL_NBR = 16;
//
//	static constexpr int _MAX_DEPTH = 5;
//
//	QuadNode root;
//};

class QuadNode
{
public:
	Math::RectangleF Bounds{ Math::Vec2F::Zero(), Math::Vec2F::Zero() };

	std::array<std::unique_ptr<QuadNode>, 4> Children{ nullptr, nullptr, nullptr, nullptr };

	CustomlyAllocatedVector<ColliderRefAabb> ColliderRefAabbs;
private:
	Allocator& Alloc;
	static constexpr int _MAX_COL_NBR = 16;

	static constexpr int _MAX_DEPTH = 5;

	int _depth = 0;

public:
	QuadNode(Allocator& alloc) noexcept;

	explicit QuadNode(const Math::RectangleF& bounds, Allocator& alloc) noexcept;

	void SetUpRoot(const Math::RectangleF& bounds) noexcept;

	void Insert(const ColliderRefAabb& colliderRefAabb) noexcept;

private:
	void Subdivide() noexcept;

};