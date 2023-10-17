//
// Created by Coch on 16.10.2023.
//
#pragma once

namespace Metrics
{
    [[nodiscard]] constexpr float PixelsToMeters(float pixels) noexcept
    {
        return pixels * 0.01f;
    }

    [[nodiscard]] constexpr float MetersToPixels(float meters) noexcept
    {
        return meters * 100.f;
    }
}
