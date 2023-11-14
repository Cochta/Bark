#include <gtest/gtest.h>
#include "World.h"
#include "Timer.h"

using namespace Math;

class WorldTest : public ::testing::Test {
protected:
	World world;
	std::vector<BodyRef> bodyRefs;
	std::vector<ColliderRef> colliderRefs;

	std::size_t initialSize = 200;

	void SetUp() override {
		world.SetUp(initialSize);
	}
	void TearDown() override {
		world.TearDown();
	}
};

TEST_F(WorldTest, AddBodyTest) {
	EXPECT_EQ(world.BodyGenIndices.size(), initialSize);
	for (int i = 0; i < initialSize * 2 + 1; ++i) {
		bodyRefs.push_back(world.CreateBody());
	}
	EXPECT_EQ(world.BodyGenIndices.size(), initialSize * 2 * 2);
}

TEST_F(WorldTest, UpdateTest) {
	float deltaTime = 0.5f;

	BodyRef bodyRef = world.CreateBody();
	Body& body = world.GetBody(bodyRef);
	body.ApplyForce(Vec2F(150.0f, 150.0f));
	body.Mass = 2.0f;
	body.Velocity = Vec2F(0.0f, 0.0f);
	body.Position = Vec2F(0.0f, 0.0f);


	Vec2F initialPosition = body.Position;
	Vec2F initialVelocity = body.Velocity;
	Vec2F initialForce = body.GetForce();

	world.Update(deltaTime);


	auto acceleration = (initialForce / body.Mass) * deltaTime;
	EXPECT_EQ(body.Velocity, initialVelocity + acceleration);
	EXPECT_FLOAT_EQ(body.Position.X, Vec2F(initialPosition + (initialVelocity + acceleration * deltaTime)).X);
	EXPECT_FLOAT_EQ(body.Position.Y, Vec2F(initialPosition + (initialVelocity + acceleration * deltaTime)).Y);
	EXPECT_EQ(body.GetForce(), Vec2F::Zero());
}

TEST_F(WorldTest, CreateAndDestroyBody) {
	// Create bodies and check if they are successfully added to the world
	for (int i = 0; i < 3; ++i) {
		BodyRef bodyRef = world.CreateBody();
		bodyRefs.push_back(bodyRef);

		EXPECT_EQ(world.BodyGenIndices.size(), initialSize);
		EXPECT_TRUE(world.GetBody(bodyRef).IsEnabled());
	}

	// Destroy bodies and check if they are removed from the world
	for (const BodyRef& bodyRef : bodyRefs) {
		world.DestroyBody(bodyRef);
		EXPECT_FALSE(world.GetBody(bodyRef).IsEnabled());
	}
	EXPECT_EQ(world.BodyGenIndices.size(), initialSize);
}

TEST_F(WorldTest, CreateAndDestroyCollider) {
	// Create bodies and colliders, and check if they are successfully added to the world
	for (int i = 0; i < 3; ++i) {
		BodyRef bodyRef = world.CreateBody();
		bodyRefs.push_back(bodyRef);

		ColliderRef colliderRef = world.CreateCollider(bodyRef);
		colliderRefs.push_back(colliderRef);

		EXPECT_EQ(world.BodyGenIndices.size(), initialSize);
		EXPECT_EQ(world.ColliderGenIndices.size(), initialSize);
		EXPECT_TRUE(world.GetCollider(colliderRef).IsAttached);
	}

	// Destroy colliders and check if they are removed from the world
	for (const ColliderRef& colliderRef : colliderRefs) {
		world.DestroyCollider(colliderRef);
		EXPECT_FALSE(world.GetCollider(colliderRef).IsAttached);
	}

	EXPECT_EQ(world.ColliderGenIndices.size(), initialSize);
	EXPECT_EQ(world.BodyGenIndices.size(), initialSize);
}