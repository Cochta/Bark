#include "QuadTree.h"

#include "gtest/gtest.h"
#include "Random.h"

using namespace Math;

static HeapAllocator HeapAlloc;

struct BoundsFixture : public ::testing::TestWithParam<RectangleF> {};

INSTANTIATE_TEST_SUITE_P(QuadNode, BoundsFixture, testing::Values(RectangleF{ Vec2F{0,0}, Vec2F{ -1,1 } },
	RectangleF{ Vec2F{ 15,13 }, Vec2F{ -12,0 } },
	RectangleF{ Vec2F{ 15.67f,-12.54f }, Vec2F{ -0,-0 } }));

TEST(QuadNode, DefaultConstructor)
{
	QuadNode node{ HeapAlloc };

	EXPECT_EQ(node.Bounds.MinBound(), Vec2F::Zero());
	EXPECT_EQ(node.Bounds.MaxBound(), Vec2F::Zero());

	for (const auto& child : node.Children)
	{
		EXPECT_EQ(child, nullptr);
	}

	EXPECT_EQ(node.ColliderRefAabbs.size(), 0);
}

TEST_P(BoundsFixture, BoundsConstructor)
{
	auto bounds = GetParam();
	QuadNode node{ bounds, HeapAlloc };

	EXPECT_EQ(node.Bounds.MinBound(), bounds.MinBound());
	EXPECT_EQ(node.Bounds.MaxBound(), bounds.MaxBound());

	for (const auto& child : node.Children)
	{
		EXPECT_EQ(child, nullptr);
	}

	EXPECT_EQ(node.ColliderRefAabbs.size(), 0);
}

TEST(QuadTree, DefaultConstructor)
{
	QuadTree tree{ HeapAlloc };
	std::size_t result = 0;
	for (size_t i = 0; i <= 5; i++) // 5 is max depth but private so can't access it and should not
	{
		result += Math::Pow(4, i);
	}
	EXPECT_EQ(tree.Nodes.size(), result);
}

TEST_P(BoundsFixture, SetUpRoot)
{
	auto bounds = GetParam();
	QuadTree tree{ HeapAlloc };
	tree.SetUpRoot(bounds);

	EXPECT_EQ(tree.Nodes[0].Bounds.MinBound(), bounds.MinBound());
	EXPECT_EQ(tree.Nodes[0].Bounds.MaxBound(), bounds.MaxBound());

	for (auto& node : tree.Nodes)
	{
		for (auto& child : node.Children)
		{
			EXPECT_TRUE(child == nullptr);
		}
	}
}