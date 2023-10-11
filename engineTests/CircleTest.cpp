//
// Created by Coch on 10.10.2023.
//
#include "gtest/gtest.h"
#include "Circle.h"


struct CircleConstructorParams {
    Vec2F position;
    float speed;
    float radius;
    Color color;
};

class CircleConstructorParamFixture : public ::testing::TestWithParam<CircleConstructorParams> {
};

INSTANTIATE_TEST_SUITE_P(CircleConstructorParams, CircleConstructorParamFixture, ::testing::Values(
        CircleConstructorParams{Vec2F(1, 2), 5.0f, 2.0f, Red},
        CircleConstructorParams{Vec2F(3, 4), 3.0f, 1.5f, Blue},
        CircleConstructorParams{Vec2F(0, 0), 0.0f, 1.0f, Black},
        CircleConstructorParams{Vec2F(-1, -1), 10.0f, 0.5f, Green},
        CircleConstructorParams{Vec2F(10, 10), 7.5f, 3.0f, White}
));

TEST_P(CircleConstructorParamFixture, ConstructorInitialization) {
    const auto params = GetParam();
    Circle circle(params.position, params.speed, params.radius, params.color);

    EXPECT_EQ(circle.Position, params.position);
    EXPECT_FLOAT_EQ(circle.Speed, params.speed);
    EXPECT_FLOAT_EQ(circle.Radius, params.radius);

    EXPECT_EQ(circle.Col.r, params.color.r);
    EXPECT_EQ(circle.Col.g, params.color.g);
    EXPECT_EQ(circle.Col.b, params.color.b);
    EXPECT_EQ(circle.Col.a, params.color.a);
}