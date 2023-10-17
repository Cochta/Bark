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
        world.Init();
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
    // Create a body and set its initial state
    BodyRef bodyRef = world.CreateBody();
    Body &body = world.GetBody(bodyRef);
    body.ApplyForce(Vec2F(150.0f, 150.0f)); // Set a force
    body.Mass = 2.0f;
    body.Velocity = Vec2F(0.0f, 0.0f);
    body.Position = Vec2F(0.0f, 0.0f);

    // Store the initial state for comparison
    Vec2F initialPosition = body.Position;
    Vec2F initialVelocity = body.Velocity;
    Vec2F initialForce = body.Force;

    // Update the world
    world.Update();

    // Check if the position and velocity have been updated as expected
    EXPECT_EQ(body.Velocity, initialVelocity + (initialForce / body.Mass) * world.Timer.DeltaTime);
    EXPECT_FLOAT_EQ(body.Position.X, Vec2F(initialPosition + (initialVelocity + ((initialForce / body.Mass) * world.Timer.DeltaTime) * world.Timer.DeltaTime)).X);
    EXPECT_FLOAT_EQ(body.Position.Y, Vec2F(initialPosition + (initialVelocity + ((initialForce / body.Mass) * world.Timer.DeltaTime) * world.Timer.DeltaTime)).Y);
    EXPECT_EQ(body.Force, Vec2F::Zero());
}