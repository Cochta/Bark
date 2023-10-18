//
// Created by Coch on 10.10.2023.
//
#include "gtest/gtest.h"
#include "Body.h"
#include "Random.h"


struct BodyConstructorParams {
    Vec2F position;
    Vec2F velocity;
    float mass;
};

class BodyConstructorParamFixture : public ::testing::TestWithParam<BodyConstructorParams> {
};

INSTANTIATE_TEST_SUITE_P(BodyConstructorParams, BodyConstructorParamFixture, ::testing::Values(
        BodyConstructorParams{Vec2F(1, 2), Vec2F(3, 4), 2.0f},
        BodyConstructorParams{Vec2F(3, 4), Vec2F(5, 6), 1.5f},
        BodyConstructorParams{Vec2F(0, 0), Vec2F(0, 0), -1.0f},
        BodyConstructorParams{Vec2F(-1, -1), Vec2F(-1, -2), 0.5f},
        BodyConstructorParams{Vec2F(10, 10), Vec2F(67, 24), 3.0f}
));

TEST_P(BodyConstructorParamFixture, ConstructorInitialization) {
    const auto params = GetParam();
    Body Body(params.position, params.velocity, params.mass);

    EXPECT_EQ(Body.Position, params.position);
    EXPECT_EQ(Body.Velocity, params.velocity);
    EXPECT_FLOAT_EQ(Body.Mass, params.mass);
}

TEST_P(BodyConstructorParamFixture, ApplyForce) {
    const auto params = GetParam();
    Body Body(params.position, params.velocity, params.mass);
    auto force = Vec2F(Random::Range(-100.f, 100.f), Random::Range(-100.f, 100.f));

    EXPECT_EQ(Body._force, Vec2F::Zero());
    Body.ApplyForce(force);
    EXPECT_EQ(Body._force, force);
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