#include <gtest/gtest.h>
#include "Allocators.h"

// Define a test fixture for LinearAllocator
class AllocatorTests : public ::testing::Test
{
protected:
    LinearAllocator LinearAlloc = {1024};
    StackAllocator StackAlloc = {1024};
    HeapAllocator HeapAlloc;
    ProxyAllocator ProxyAlloc{HeapAlloc};
    FreeListAllocator FreeListAlloc{1024};
    PoolAllocator PoolAlloc{10, sizeof(int)};

    void SetUp() override
    {

    }

    void TearDown() override
    {

    }
};

// Test the Allocate function
TEST_F(AllocatorTests, LinearAllocate)
{
    void *ptr = LinearAlloc.Allocate(4, sizeof(int));

    EXPECT_TRUE(ptr != nullptr);
}

// Test the Reset function
TEST_F(AllocatorTests, LinearReset)
{
    void *ptr = LinearAlloc.Allocate(4, sizeof(int));

    LinearAlloc.Reset();

    void *newPtr = LinearAlloc.Allocate(4, sizeof(int));

    EXPECT_TRUE(newPtr != nullptr);
}

// Test the Allocate and Deallocate functions
TEST_F(AllocatorTests, StackAllocateAndDeallocate)
{
    void *ptr = StackAlloc.Allocate(4, sizeof(int));

    EXPECT_TRUE(ptr != nullptr);

    StackAlloc.Deallocate(ptr);
}

// Test the Reset function
TEST_F(AllocatorTests, StackReset)
{
    void *ptr = StackAlloc.Allocate(4, sizeof(int));

    StackAlloc.Reset();

    void *newPtr = StackAlloc.Allocate(4, sizeof(int));

    EXPECT_TRUE(newPtr != nullptr);
}

TEST_F(AllocatorTests, HeapAllocateAndDeallocate) {
    void* ptr = HeapAlloc.Allocate(4, sizeof(int));

    EXPECT_TRUE(ptr != nullptr);

    HeapAlloc.Deallocate(ptr);
}

TEST_F(AllocatorTests, ProxyAllocateAndDeallocate) {
    void* ptr = ProxyAlloc.Allocate(4, sizeof(int));

    EXPECT_TRUE(ptr != nullptr);

    ProxyAlloc.Deallocate(ptr);
}

TEST_F(AllocatorTests, FreeListAllocateAndDeallocate) {
    void* ptr = FreeListAlloc.Allocate(4, sizeof(int));

    EXPECT_TRUE(ptr != nullptr);

    FreeListAlloc.Deallocate(ptr);
}

TEST_F(AllocatorTests, FreeListReset) {
    void* ptr = FreeListAlloc.Allocate(4, sizeof(int));

    FreeListAlloc.Reset();

    void* newPtr = FreeListAlloc.Allocate(4, sizeof(int));

    EXPECT_TRUE(newPtr != nullptr);
}
TEST_F(AllocatorTests, PoolAllocateAndDeallocate) {
    void* ptr = PoolAlloc.Allocate(1, sizeof(int)); // Allocate a single int

    EXPECT_TRUE(ptr != nullptr);

    PoolAlloc.Deallocate(ptr);
}

TEST_F(AllocatorTests, PoolReset) {
    void* ptr = PoolAlloc.Allocate(1, sizeof(int)); // Allocate a single int
    PoolAlloc.Reset();

    void* newPtr = PoolAlloc.Allocate(1, sizeof(int));

    EXPECT_TRUE(newPtr != nullptr);
}