#pragma once

#include <utility>
#include <vector>

constexpr static float Epsilon = 0.00001f;
const double e =  2.71828;

template <typename T>
[[nodiscard]] constexpr T Abs(T nbr) noexcept
{
    return nbr < 0 ? -nbr : nbr;
}

template<typename T>
[[nodiscard]] constexpr T Fact(T n) noexcept {
    T result = 1;

    for (int i = 2; i <= Abs(n); i++) {
        result *= i;
    }

    if (n < 0) {
        result = -result;
    }

    return result;
}

[[nodiscard]] constexpr double GetEulerNumber(int taylorIter) noexcept {
    double result = 0.f;

    for (int i = 0; i < taylorIter; ++i) {
        result += 1.0f / (double)Fact(i);
    }

    return result;
}