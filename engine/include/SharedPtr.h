//
// Created by Coch on 14.10.2023.
//

#pragma once

template<typename T>
class SharedPtr
{
private:
    T *_ptr;
    std::size_t *_ref_count;
public:
    [[nodiscard]] constexpr SharedPtr() noexcept: _ptr(nullptr), _ref_count(nullptr)
    {}

    [[nodiscard]] constexpr explicit SharedPtr(T *ptr) noexcept: _ptr(ptr)
    {
        _ref_count = new std::size_t(1);
    }

    [[nodiscard]] SharedPtr(const SharedPtr &other) noexcept: _ptr(other._ptr), _ref_count(other._ref_count)
    {
        if (_ref_count)
        {
            (*_ref_count)++;
        }
    }

    SharedPtr &operator=(const SharedPtr &other) noexcept
    {
        if (this != &other)
        {
            if (_ref_count && --(*_ref_count) == 0)
            {
                delete _ptr;
                delete _ref_count;
            }

            _ptr = other._ptr;
            _ref_count = other._ref_count;

            if (_ref_count)
            {
                (*_ref_count)++;
            }
        }

        return *this;
    }

    ~SharedPtr() noexcept
    {
        if (_ref_count && --(*_ref_count) == 0)
        {
            delete _ptr;
            delete _ref_count;
        }
    }

    [[nodiscard]] constexpr T *Get() const noexcept
    {
        return _ptr;
    }

    [[nodiscard]] constexpr std::size_t UseCount() const noexcept
    {
        if (_ref_count)
        {
            return *_ref_count;
        }
        return 0;
    }
};