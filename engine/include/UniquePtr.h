//
// Created by Coch on 14.10.2023.
//
#pragma once

template<typename T>
class UniquePtr
{
private:
    T *Ptr;

public:

    constexpr explicit UniquePtr(T *ptr) noexcept: Ptr(ptr)
    {}

    // Disable default constructor, copy constructor and copy assignment
    UniquePtr() = delete;

    UniquePtr(const UniquePtr &) = delete;

    UniquePtr &operator=(const UniquePtr &) = delete;

    ~UniquePtr() noexcept
    {
        delete Ptr;
    }

    // Move constructor: Transfer ownership of the managed pointer
    constexpr UniquePtr(UniquePtr &&other) noexcept
    {
        Ptr = other.Ptr;
        other.Ptr = nullptr;
    }

    // Move assignment: Transfer ownership of the managed pointer
    constexpr UniquePtr &operator=(UniquePtr &&other) noexcept
    {
        if (this != &other)
        {
            delete Ptr;
            Ptr = other.Ptr;
            other.Ptr = nullptr;
        }
        return *this;
    }


    // Dereference operator
    constexpr T &operator*() const noexcept
    {
        return *Ptr;
    }

    // Member access operator
    constexpr T *operator->() const noexcept
    {
        return Ptr;
    }

    // Get the raw pointer
    constexpr T *Get() const noexcept
    {
        return Ptr;
    }

    // Check if the UniquePtr is managing a pointer
    [[nodiscard]] constexpr bool IsNull() const noexcept
    {
        return Ptr == nullptr;
    }
};