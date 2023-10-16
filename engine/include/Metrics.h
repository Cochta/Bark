//
// Created by Coch on 16.10.2023.
//
#pragma once

namespace Metrics
{
    struct Pixel
    {
        constexpr explicit Pixel(float value) noexcept: Value(value)
        {};
        float Value;
    };

    struct Meter
    {
        constexpr explicit Meter(float value) noexcept: Value(value)
        {};
        float Value;
    };

    [[nodiscard]] constexpr Meter PixelsToMeters(Pixel pixels) noexcept
    {
        return Meter(pixels.Value * 0.01f);
    }

    [[nodiscard]] constexpr Pixel MetersToPixels(Meter meters) noexcept
    {
        return Pixel(meters.Value / 0.01f);
    }

    [[nodiscard]] constexpr float PixelsToMeters(float pixels) noexcept
    {
        return pixels * 0.01f;
    }

    [[nodiscard]] constexpr float MetersToPixels(float meters) noexcept
    {
        return meters / 0.01f;
    }
}
