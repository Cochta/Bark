#include "gtest/gtest.h"
#include "Collider.h"

using namespace Math;

class ColliderFixture : public ::testing::Test {
public:
    std::vector<CircleF> circles;
    std::vector<RectangleF> rectangles;
   // std::vector<PolygonF> polygons;

    int nbColliders = 3;
protected:
    void SetUp() override {
        circles.emplace_back(Vec2F::Zero(),0.f);
        circles.emplace_back(-Vec2F::One(),-5.f);
        circles.emplace_back(Vec2F::Right(),5.f);
        rectangles.emplace_back(Vec2F::Zero(),Vec2F::Zero());
        rectangles.emplace_back(Vec2F::One(),Vec2F::One());
        rectangles.emplace_back(-Vec2F::Right(),-Vec2F::Left());
    }

    void TearDown() override {
        // Clean up any shared resources or perform teardown here.
    }
};
TEST_F(ColliderFixture, DefaultConstructor) {
    Collider collider;

    ASSERT_TRUE(std::holds_alternative<CircleF>(collider.Shape));
    ASSERT_EQ(collider.IsSensor, true);
    ASSERT_EQ(collider.IsAttached, false);
}

TEST_F(ColliderFixture, CircleConstructor) {
    for(int i = 0; i < nbColliders ;i++)
    {
        Collider collider;
        collider.
        Shape = circles[i];
        // Add assertions to test the Circle constructor.
        ASSERT_TRUE(std::holds_alternative<CircleF>(collider.Shape)
        );
        ASSERT_EQ(std::get<CircleF>(collider.Shape), circles[0]
        );
        ASSERT_EQ(collider
        .IsSensor, true);
        ASSERT_EQ(collider
        .IsAttached, false);
        // Add more assertions as needed.
    }
}
//
//TEST_F(ColliderFixture, RectangleConstructor) {
//Collider collider(rectangles[1]);
//// Add assertions to test the Rectangle constructor.
//ASSERT_TRUE(std::holds_alternative<RectangleF>(collider.Shape));
//ASSERT_EQ(std::get<RectangleF>(collider.Shape), rectangles[1]);
//ASSERT_EQ(collider.IsSensor, true);
//ASSERT_EQ(collider.IsAttached, false);
//// Add more assertions as needed.
//}