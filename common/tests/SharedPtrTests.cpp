//
// Created by Coch on 14.10.2023.
//
#include <gtest/gtest.h>
#include "../include/SharedPtr.h"

struct SharedPtrTestFixture : public ::testing::Test {
    SharedPtr<int> p1;
    SharedPtr<int> p2;
    SharedPtr<int> p3;

    void SetUp() override {
        p1 = SharedPtr<int>(new int(42));
        p2 = p1;
        p3 = p1;
    }

    void TearDown() override {
    }
};

TEST_F(SharedPtrTestFixture, ConstructionWithUseCount) {
    EXPECT_EQ(p1.UseCount(), 3);
    EXPECT_EQ(p2.UseCount(), 3);
    EXPECT_EQ(p3.UseCount(), 3);

    EXPECT_EQ(*p1.Get(), 42);
    EXPECT_EQ(*p2.Get(), 42);
    EXPECT_EQ(*p3.Get(), 42);
}

TEST_F(SharedPtrTestFixture, AssignmentWithUseCount) {
    SharedPtr<int> p4 = p1;
    EXPECT_EQ(p1.UseCount(), 4);
    EXPECT_EQ(p2.UseCount(), 4);
    EXPECT_EQ(p3.UseCount(), 4);
    EXPECT_EQ(p4.UseCount(), 4);

    p4 = p2;
    EXPECT_EQ(p1.UseCount(), 4);
    EXPECT_EQ(p2.UseCount(), 4);
    EXPECT_EQ(p3.UseCount(), 4);
    EXPECT_EQ(p4.UseCount(), 4);

    p4 = SharedPtr<int>(new int(99));
    EXPECT_EQ(p1.UseCount(), 3);
    EXPECT_EQ(p2.UseCount(), 3);
    EXPECT_EQ(p3.UseCount(), 3);
    EXPECT_EQ(p4.UseCount(), 1);

    EXPECT_EQ(*p1.Get(), 42);
    EXPECT_EQ(*p2.Get(), 42);
    EXPECT_EQ(*p3.Get(), 42);
    EXPECT_EQ(*p4.Get(), 99);
}

TEST_F(SharedPtrTestFixture, DestructorWithUseCount) {

}