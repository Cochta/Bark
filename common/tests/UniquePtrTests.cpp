#include <gtest/gtest.h>
#include "UniquePtr.h"

// Define a test fixture for UniquePtr
class UniquePtrTest : public ::testing::Test
{
protected:
    // Create UniquePtr instances with different data
    UniquePtr<int> *nullPtr;
    UniquePtr<int> *initializedPtr;
    UniquePtr<int> *movedPtr;

    void SetUp() override
    {
        initializedPtr = new UniquePtr<int>(new int(42));

        nullPtr = new UniquePtr<int>(nullptr);
        movedPtr = new UniquePtr<int>(new int(17));
    }

    void TearDown() override
    {
        delete nullPtr;
        delete initializedPtr;
        delete movedPtr;
    }
};

TEST_F(UniquePtrTest, NullUniquePtr)
{
    EXPECT_TRUE(nullPtr->IsNull());
    EXPECT_FALSE(initializedPtr->IsNull());
}

TEST_F(UniquePtrTest, InitializedUniquePtr)
{
    EXPECT_FALSE(initializedPtr->IsNull());
    EXPECT_EQ(**initializedPtr, 42);
}

TEST_F(UniquePtrTest, MoveConstructor)
{
    EXPECT_FALSE(movedPtr->IsNull());
    UniquePtr<int> moved = std::move(*movedPtr);
    EXPECT_TRUE(movedPtr->IsNull());
    EXPECT_FALSE(moved.IsNull());
    EXPECT_EQ(*moved, 17);
}

TEST_F(UniquePtrTest, MoveAssignment)
{
    UniquePtr<int> movedAss(nullptr);
    movedAss = std::move(*movedPtr);
    EXPECT_TRUE(movedPtr->IsNull());
    EXPECT_FALSE(movedAss.IsNull());
    EXPECT_EQ(*movedAss, 17);
}

TEST_F(UniquePtrTest, DereferenceOperator)
{
    EXPECT_EQ(**initializedPtr, 42);
}

TEST_F(UniquePtrTest, MemberAccessOperator)
{
    EXPECT_EQ((*initializedPtr).Get(), initializedPtr->Get());
}