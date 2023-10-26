#pragma once

#include "Sample.h"
#include "Random.h"

class StarSystemSample : public Sample
{
private:
    std::vector<Math::CircleF> _circles;

    BodyRef _sunRef;

    static constexpr float G = 6.67f;
    static constexpr std::size_t PLANET_NBR = 10000;
protected:
    void SampleSetUp() noexcept override;

    void SampleUpdate() noexcept override;

    void SampleTearDown() noexcept override;

private:
    static void CalculateGravitationalForce(const Body &sun, Body &body) noexcept;

};
