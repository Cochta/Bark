//
// Created by Coch on 16.10.2023.
//
#include <gtest/gtest.h>
#include "World.h"

class WorldTest : public ::testing::Test {
protected:
    World world;

    void SetUp() override {
        Body b1(Vec2F(200,-400),Vec2F(20,40), 1);
        Body b2(Vec2F(0,-0),Vec2F(0,-0), 0);
        Body b3(Vec2F(234.457,-123.1254),Vec2F(12.45376,-40.1214), -1245);
        world.Init();
        world.CreateBody(b1);
        world.CreateBody(b2);
        world.CreateBody(b3);
    }
};

TEST_F(WorldTest, AddBodyTest) {

    ASSERT_EQ(world.Bodies.size(), 3);

}

TEST_F(WorldTest, UpdateTest) {
    int nbUpdates = 3;
    //World TestWorld = world;
    //TestWorld.Init();
    world.Update();

    for (int i = 0; i < nbUpdates; ++i)
    {

    }
//
//    for (int i = 0; i < world._bodies.size(); ++i)
//    {
//        Body body = world._bodies[i];
//        Body testBody = TestWorld._bodies[i];
//        ASSERT_EQ(body.Mass, testBody.Mass);
//    }

}