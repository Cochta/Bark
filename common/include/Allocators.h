#pragma once

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

class Allocator
{
public:
    virtual ~Allocator() = default;

    virtual void *Allocate(std::size_t count, std::size_t typeSize) = 0;

    virtual void Deallocate(void *ptr) = 0;
};

class LinearAllocator : public Allocator
{
public:
    LinearAllocator(std::size_t size) : _size(size), _ptr(new char[size]), _offset(0)//todo: donner juste le pointer
    {}

    ~LinearAllocator() override
    {
        delete[] _ptr;
    }

    void *Allocate(std::size_t size, std::size_t typeSize) override // todo: ajouter alignement
    {
        if (_offset + size * typeSize > _size)
        {
            throw std::runtime_error("LinearAllocator out of memory");
        }
        void *ptr = _ptr + _offset;
        _offset += size * typeSize;
        return ptr;
    }

    void Deallocate(void *ptr) override
    {}

    void Reset()
    {
        _offset = 0;
    }

private:
    std::size_t _size;
    char *_ptr = nullptr;
    std::size_t _offset;
};

struct AllocationHeader
{
    std::size_t size;
};

class StackAllocator : public Allocator
{
public:
    StackAllocator(std::size_t size) : _size(size), _ptr(new char[size]), _top(0)
    {}

    ~StackAllocator() override
    {
        delete[] _ptr;
    }

    void *Allocate(std::size_t size, std::size_t typeSize) override
    {
        if (_top + size * typeSize > _size)
        {
            throw std::runtime_error("StackAllocator out of memory");
        }

        // Calculate the size of the allocation header
        std::size_t headerSize = sizeof(AllocationHeader);

        // Calculate the total size needed for the allocation
        std::size_t totalSize = size * typeSize + headerSize;

        // Get a pointer to the memory for this allocation
        char *allocationPtr = _ptr + _top;

        // Set the allocation header
        AllocationHeader *header = reinterpret_cast<AllocationHeader *>(allocationPtr);
        header->size = totalSize;

        // Move the top of the stack to the next available memory location
        _top += totalSize;

        // Return a pointer to the user data (after the header)
        return allocationPtr + headerSize;
    }

    void Deallocate(void *ptr) override // todo: checker que celui a desalouer soit le top
    {
        // To deallocate, we use the allocation header to get the size
        // and then move the top of the stack back by that amount.
        char *allocationPtr = reinterpret_cast<char *>(ptr) - sizeof(AllocationHeader);
        AllocationHeader *header = reinterpret_cast<AllocationHeader *>(allocationPtr);
        _top -= header->size;
    }

    void Reset()
    {
        _top = 0;
    }

private:
    std::size_t _size;
    char *_ptr;
    std::size_t _top;
};

class HeapAllocator final : public Allocator
{
public:
    void *Allocate(std::size_t size, std::size_t typeSize) override
    {
        return ::operator new(size * typeSize);
    }

    void Deallocate(void *ptr) override
    {
        ::operator delete(ptr);
    }
};

class ProxyAllocator : public Allocator
{
public:
    ProxyAllocator(Allocator &targetAllocator) : _targetAllocator(targetAllocator)
    {}

    void *Allocate(std::size_t count, std::size_t typeSize) override
    {
        return _targetAllocator.Allocate(count, typeSize);
    }

    void Deallocate(void *ptr) override
    {
        // Forward the deallocation request to the wrapped allocator
        _targetAllocator.Deallocate(ptr);
    }

private:
    Allocator &_targetAllocator;
};

struct FreeBlock
{
    std::size_t size;
    FreeBlock *next;
};

class FreeListAllocator : public Allocator
{
public:
    FreeListAllocator(std::size_t size) : _size(size), _ptr(new char[size]), _freeList(_ptr)
    {
        FreeBlock *firstBlock = reinterpret_cast<FreeBlock *>(_freeList);
        firstBlock->size = _size;
        firstBlock->next = nullptr;
    }

    ~FreeListAllocator() override
    {
        delete[] _ptr;
    }

    void *Allocate(std::size_t count, std::size_t typeSize) override
    {
        FreeBlock *prev = nullptr;
        FreeBlock *current = reinterpret_cast<FreeBlock *>(_freeList);

        while (current)
        {
            if (current->size >= count * typeSize)
            {
                if (current->size > count * typeSize)
                {
                    // Split the block if it's larger than needed
                    char *newBlockPtr = reinterpret_cast<char *>(current) + count * typeSize;
                    FreeBlock *newBlockHeader = reinterpret_cast<FreeBlock *>(newBlockPtr);
                    newBlockHeader->size = current->size - count * typeSize;
                    newBlockHeader->next = current->next;
                }

                // Update the free list
                if (prev)
                {
                    prev->next = current->next;
                } else
                {
                    _freeList = reinterpret_cast<char *>(current) + count * typeSize;
                }

                // Return a pointer to the user data (after the header)
                return current + 1;
            }

            prev = current;
            current = current->next;
        }

        throw std::runtime_error("FreeListAllocator out of memory");
    }

    void Deallocate(void *ptr) override
    {
        if (!ptr)
        {
            return;
        }

        char *blockPtr = reinterpret_cast<char *>(ptr) - sizeof(FreeBlock);
        FreeBlock *firstBlock = reinterpret_cast<FreeBlock *>(blockPtr);

        // Insert the deallocated block into the free list
        firstBlock->next = reinterpret_cast<FreeBlock *>(_freeList);
        _freeList = blockPtr;
    }

    void Reset()
    {
        FreeBlock *firstBlock = reinterpret_cast<FreeBlock *>(_freeList);
        firstBlock->size = _size;
        firstBlock->next = nullptr;
    }

private:
    std::size_t _size;
    char *_ptr;
    char *_freeList;
};

struct PoolBlock
{
    PoolBlock *next;
};

class PoolAllocator : public Allocator
{
public:
    PoolAllocator(std::size_t blockCount, std::size_t blockSize) : _blockSize(blockSize), _blockCount(blockCount)
    {
        _pool = new char[blockSize * blockCount];
        InitializePoolList();
    }

    ~PoolAllocator() override
    {
        delete[] _pool;
    }

    void *Allocate(std::size_t count, std::size_t typeSize) override
    {
        if (count * typeSize != _blockSize)
        {
            throw std::runtime_error("Invalid allocation size for PoolAllocator");
        }

        if (_poolList == nullptr)
        {
            throw std::runtime_error("PoolAllocator out of memory");
        }

        PoolBlock *allocatedBlock = _poolList;
        _poolList = _poolList->next;
        return allocatedBlock;
    }

    void Deallocate(void *ptr) override
    {
        if (!ptr)
        {
            return;
        }

        PoolBlock *deallocatedBlock = reinterpret_cast<PoolBlock *>(ptr);
        deallocatedBlock->next = _poolList;
        _poolList = deallocatedBlock;
    }

    void Reset()
    {
        InitializePoolList();
    }

private:
    std::size_t _blockSize;
    std::size_t _blockCount;
    char *_pool;
    PoolBlock *_poolList;

    void InitializePoolList()
    {
        _poolList = nullptr;

        for (std::size_t i = 0; i < _blockCount; ++i)
        {
            char *blockPtr = _pool + i * _blockSize;
            PoolBlock *blockFirst = reinterpret_cast<PoolBlock *>(blockPtr);
            blockFirst->next = _poolList;
            _poolList = blockFirst;
        }
    }
};