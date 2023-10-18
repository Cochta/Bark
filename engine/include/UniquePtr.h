//
// Created by Coch on 14.10.2023.
//
#pragma once

template<typename T>
class UniquePtr {
private:
    T *_ptr;

public:

    constexpr explicit UniquePtr(T *ptr) noexcept: _ptr(ptr) {}

    // Disable default constructor, copy constructor and copy assignment
    UniquePtr() = delete;

    UniquePtr(const UniquePtr &) = delete;

    UniquePtr &operator=(const UniquePtr &) = delete;

    ~UniquePtr() noexcept {
        delete _ptr;
    }

    // Move constructor: Transfer ownership of the managed pointer
    constexpr UniquePtr(UniquePtr &&other) noexcept {
        _ptr = other._ptr;
        other._ptr = nullptr;
        //todo: check for std::Swap(Ptr, other.Ptr);
    }

    // Move assignment: Transfer ownership of the managed pointer
    constexpr UniquePtr &operator=(UniquePtr &&other) noexcept {
        if (this != &other) {
            delete _ptr;
            _ptr = other._ptr;
            other._ptr = nullptr;
        }
        //todo: check for std::Swap(Ptr, other.Ptr);
        return *this;
    }


    // Dereference operator
    constexpr T &operator*() const noexcept {
        return *_ptr;
    }

    // Member access operator
    constexpr T *operator->() const noexcept {
        return _ptr;
    }

    // Get the raw pointer
    constexpr T *Get() const noexcept {
        return _ptr;
    }

    // Check if the UniquePtr is managing a pointer
    [[nodiscard]] constexpr bool IsNull() const noexcept {
        return _ptr == nullptr;
    }

    template<typename U>
    operator UniquePtr<U>() {
        auto n = _ptr;
        _ptr = nullptr;

        return UniquePtr<U>(n);
    }
};

template<typename T>
// todo: implement with args
UniquePtr<T> MakeUnique(T value) noexcept {
    return UniquePtr<T>(new T(value));
}