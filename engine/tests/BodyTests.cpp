#include "gtest/gtest.h"
#include "Body.h"
#include "Random.h"

using namespace Math;

struct BodyConstructorParams {
    Vec2F position;
    Vec2F velocity;
    float mass;
    Vec2F force;
};

class BodyConstructorParamFixture : public ::testing::TestWithParam<BodyConstructorParams> {
};

INSTANTIATE_TEST_SUITE_P(BodyConstructorParams, BodyConstructorParamFixture, ::testing::Values(
        BodyConstructorParams{Vec2F(1, 2), Vec2F(3, 4), 2.0f, Vec2F(15.f, 2.f)},
        BodyConstructorParams{Vec2F(3, 4), Vec2F(5, 6), 1.5f, Vec2F(4.15f, 2.76f) },
        BodyConstructorParams{Vec2F(0, 0), Vec2F(0, 0), -1.0f, Vec2F(-12.f, -121.f) },
        BodyConstructorParams{Vec2F(-1, -1), Vec2F(-1, -2), 0.5f, Vec2F(0.f, -0.f)},
        BodyConstructorParams{Vec2F(10, 10), Vec2F(67, 24), 3.0f, Vec2F(47.f, 27.f) }
));

TEST_P(BodyConstructorParamFixture, ConstructorInitialization) {
    const auto params = GetParam();
    Body Body(params.position, params.velocity, params.mass);

    EXPECT_EQ(Body.Position, params.position);
    EXPECT_EQ(Body.Velocity, params.velocity);
    EXPECT_FLOAT_EQ(Body.Mass, params.mass);
}

TEST_P(BodyConstructorParamFixture, IsEnabled) {
    const auto params = GetParam();
    Body Body(params.position, params.velocity, params.mass);
    if (Body.Mass >= 0) {
        EXPECT_TRUE(Body.IsEnabled());
    } else {
        EXPECT_FALSE(Body.IsEnabled());
    }
}

TEST_P(BodyConstructorParamFixture, ApplyGetResetForce) {
    const auto params = GetParam();
    Body body(params.position, params.velocity, params.mass);
    body.ApplyForce(params.force);
    EXPECT_EQ(body.GetForce(), params.force);
    body.ResetForce();
    EXPECT_EQ(body.GetForce(), Vec2F::Zero());
}