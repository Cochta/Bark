//
// Created by Coch on 16.10.2023.
//
#include <gtest/gtest.h>
#include "World.h"

class WorldTest : public ::testing::Test {
protected:
    World world;
    std::vector<BodyRef> bodyRefs;

    void SetUp() override {
        world.SetUp();
    }
};

TEST_F(WorldTest, AddBodyTest) {
    int baseSize = 100;
    EXPECT_EQ(world.GenIndices.size(), baseSize);
    for (int i = 0; i < 250; ++i) {
        bodyRefs.push_back(world.CreateBody());
    }
    EXPECT_EQ(world.GenIndices.size(), baseSize * 2 * 2);
}

TEST_F(WorldTest, UpdateTest) {
    BodyRef bodyRef = world.CreateBody();
    Body &body = world.GetBody(bodyRef);
    body.ApplyForce(Vec2F(150.0f, 150.0f));
    body.Mass = 2.0f;
    body.Velocity = Vec2F(0.0f, 0.0f);
    body.Position = Vec2F(0.0f, 0.0f);


    Vec2F initialPosition = body.Position;
    Vec2F initialVelocity = body.Velocity;
    Vec2F initialForce = body._force;


    world.Update();

    auto acceleration = (initialForce / body.Mass) * world._timer.DeltaTime;
    EXPECT_EQ(body.Velocity, initialVelocity + acceleration);
    EXPECT_FLOAT_EQ(body.Position.X, Vec2F(initialPosition + (initialVelocity + acceleration * world.Timer.DeltaTime)).X);
    EXPECT_FLOAT_EQ(body.Position.Y, Vec2F(initialPosition + (initialVelocity + acceleration * world.Timer.DeltaTime)).Y);
    EXPECT_EQ(body._force, Vec2F::Zero());
}