//
// Created by Coch on 10.10.2023.
//
#include "Metrics.h"

#include "Vec2.h"

class Body {
public:
    Vec2F Position = Vec2F::Zero();
    Vec2F Velocity = Vec2F::Zero();
    float Mass = -1.f; // Body is disable if mass is negative
    Vec2F _force = Vec2F::Zero(); // Total force acting on the body
private:


public:
    constexpr Body() noexcept = default;

    constexpr explicit Body(Vec2F position, Vec2F velocity, float mass) noexcept
            : Position(position), Velocity(velocity), Mass(mass) {}

    void ApplyForce(const Vec2F &force) noexcept;

    [[nodiscard]] constexpr bool IsEnabled() const noexcept { return Mass >= 0; }

    constexpr void Enable() noexcept { Mass = 1.f; }

    constexpr void Disable() noexcept { Mass = -1.f; }

    [[nodiscard]] constexpr Vec2F GetForce() const { return _force; }

    constexpr void ResetForce() { _force = Vec2F::Zero(); }
};