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
	std::array<std::unique_ptr<QuadNode>, 4> Children{ nullptr, nullptr, nullptr, nullptr };
	UniquePtr<Allocator> HeapAlloc = UniquePtr<Allocator>(new HeapAllocator());
	CustomlyAllocatedVector<ColliderRefAabb> ColliderRefAabbs{ *HeapAlloc };
private:
	static constexpr int _MAX_COL_NBR = 1000;

	static constexpr int _MAX_DEPTH = 5;

	int _depth = 0;

public:
	QuadNode() noexcept = default;

	explicit QuadNode(const Math::RectangleF& bounds) noexcept : Bounds(bounds)
	{};

	void SetUpRoot(const Math::RectangleF& bounds) noexcept;

	void Insert(const ColliderRefAabb& colliderRefAabb) noexcept;

private:
	void Subdivide() noexcept;

};