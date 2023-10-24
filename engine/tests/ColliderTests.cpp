#include "gtest/gtest.h"
#include "Collider.h"

using namespace Math;

// Test fixture for Collider class
class ColliderTest : public ::testing::Test {
protected:
    Collider collider;

    void SetUp() override {
        collider.ID = 1; // Set a unique ID for each test
    }
};

TEST_F(ColliderTest, DefaultValues) {
    // Check default values for a new collider
    EXPECT_EQ(collider.ColShape, nullptr);
    EXPECT_TRUE(collider.IsSensor);
    EXPECT_FALSE(collider.IsAttached);
    EXPECT_EQ(collider.ID, 1);
    EXPECT_FALSE(collider.DoesTrigger());
}

TEST_F(ColliderTest, TriggerFunctions) {
    // Test the OnTriggerEnter and OnTriggerExit functions
    EXPECT_FALSE(collider.DoesTrigger());

    collider.OnTriggerEnter();
    EXPECT_TRUE(collider.DoesTrigger());

    collider.OnTriggerExit();
    EXPECT_FALSE(collider.DoesTrigger());
}