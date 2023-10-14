//
// Created by Coch on 14.10.2023.
//

#pragma once

template<typename T>
class SharedPtr
{
public:
    [[nodiscard]] constexpr SharedPtr() noexcept: ptr(nullptr), ref_count(nullptr)
    {}

    [[nodiscard]] constexpr explicit SharedPtr(T *ptr) noexcept: ptr(ptr)
    {
        ref_count = new size_t(1);
    }

    [[nodiscard]] SharedPtr(const SharedPtr &other) noexcept: ptr(other.ptr), ref_count(other.ref_count)
    {
        if (ref_count)
        {
            (*ref_count)++;
        }
    }

    SharedPtr &operator=(const SharedPtr &other) noexcept
    {
        if (this != &other)
        {
            if (ref_count && --(*ref_count) == 0)
            {
                delete ptr;
                delete ref_count;
            }

            ptr = other.ptr;
            ref_count = other.ref_count;

            if (ref_count)
            {
                (*ref_count)++;
            }
        }

        return *this;
    }

    ~SharedPtr() noexcept
    {
        if (ref_count && --(*ref_count) == 0)
        {
            delete ptr;
            delete ref_count;
        }
    }

    [[nodiscard]] constexpr T *Get() const noexcept
    {
        return ptr;
    }

    [[nodiscard]] constexpr size_t UsedCount() const noexcept
    {
        if (ref_count)
        {
            return *ref_count;
        }
        return 0;
    }

private:
    T *ptr;
    size_t *ref_count;
};