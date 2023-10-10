#pragma once

#include <utility>
#include <vector>
#include "../libs/Math922/include/MathUtility.h"

const double e =  2.718281828459045; // 15 decimales

[[nodiscard]] constexpr double GetEulerNumber(int taylorIter) noexcept {
    double result = 0.f;

    for (int i = 0; i < taylorIter; ++i) {
        result += 1.0f / (double)MathUtility::Fact((double)i);
    }

    return result;
}