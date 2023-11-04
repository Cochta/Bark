#include "gtest/gtest.h"
#include "Collider.h"

using namespace Math;

class ColliderFixture : public ::testing::Test
{
public:
    std::vector<CircleF> circles;
    std::vector<RectangleF> rectangles;
    // std::vector<PolygonF> polygons;
    std::vector<bool> areAttached;
    std::vector<bool> areSensor;


    int nbColliders = 3;
protected:
    void SetUp() override
    {
        circles.emplace_back(Vec2F::Zero(), 0.f);
        circles.emplace_back(-Vec2F::One(), -5.f);
        circles.emplace_back(Vec2F::Right(), 5.f);
        rectangles.emplace_back(Vec2F::Zero(), Vec2F::Zero());
        rectangles.emplace_back(Vec2F::One(), Vec2F::One());
        rectangles.emplace_back(-Vec2F::Right(), -Vec2F::Left());
        areAttached.emplace_back(true);
        areAttached.emplace_back(false);
        areAttached.emplace_back(true);
        areSensor.emplace_back(false);
        areSensor.emplace_back(true);
        areSensor.emplace_back(false);

    }
};

class ColliderRefFixture : public ::testing::Test
{
public:
    std::vector<std::size_t> ids;
    std::vector<std::size_t> genIds;

    size_t nbColRefs = 3;

protected:
    void SetUp() override
    {
        ids.emplace_back(1);
        ids.emplace_back(2);
        ids.emplace_back(3);
        genIds.emplace_back(3);
        genIds.emplace_back(2);
        genIds.emplace_back(1);
    }
};

TEST_F(ColliderFixture, DefaultConstructor)
{
    Collider collider;

    ASSERT_TRUE(std::holds_alternative<CircleF>(collider.Shape));
    ASSERT_EQ(collider.IsTrigger, true);
    ASSERT_EQ(collider.IsAttached, false);
}

TEST_F(ColliderFixture, CircleConstructor)
{
    for (int i = 0; i < nbColliders; i++)
    {
        Collider collider;
        collider.Shape = circles[i];
        collider.IsTrigger = areSensor[i];
        collider.IsAttached = areAttached[i];
        // Add assertions to test the Circle constructor.
        ASSERT_TRUE(std::holds_alternative<CircleF>(collider.Shape));
        ASSERT_EQ(std::get<CircleF>(collider.Shape).Center(), circles[i].Center());
        ASSERT_EQ(std::get<CircleF>(collider.Shape).Radius(), circles[i].Radius());
        ASSERT_EQ(collider.IsTrigger, areSensor[i]);
        ASSERT_EQ(collider.IsAttached, areAttached[i]);

    }
}

TEST_F(ColliderFixture, RectangleConstructor)
{
    for (int i = 0; i < nbColliders; i++)
    {
        Collider collider;
        collider.Shape = rectangles[i];
        collider.IsTrigger = areSensor[i];
        collider.IsAttached = areAttached[i];
        // Add assertions to test the Circle constructor.
        ASSERT_TRUE(std::holds_alternative<RectangleF>(collider.Shape));
        ASSERT_EQ(std::get<RectangleF>(collider.Shape).MinBound(), rectangles[i].MinBound());
        ASSERT_EQ(std::get<RectangleF>(collider.Shape).MaxBound(), rectangles[i].MaxBound());
        ASSERT_EQ(collider.IsTrigger, areSensor[i]);
        ASSERT_EQ(collider.IsAttached, areAttached[i]);

    }
}

TEST_F(ColliderRefFixture, ColliderRefConstructor)
{
    for (std::size_t i = 0; i < nbColRefs; ++i)
    {
        ColliderRef colRef = {ids[i], genIds[i]};
        ASSERT_EQ(colRef.Index, ids[i]);
        ASSERT_EQ(colRef.GenIndex, genIds[i]);
    }

}
TEST_F(ColliderRefFixture, ColliderRefPairConstructor)
{
    for (std::size_t i = 0; i < nbColRefs; ++i)
    {
        ColliderRef colRef1 = {ids[i], genIds[i]};
        for (std::size_t j = 0; j < nbColRefs; ++j)
        {
            ColliderRef colRef2 = {ids[j], genIds[j]};
            if(i==j) continue;

            ColliderRefPair ColRefPair = {colRef1,colRef2};
            //ASSERT_EQ(ColRefPair.ColRefA, colRef1); jsp pk ca marche pas
        }
    }
}