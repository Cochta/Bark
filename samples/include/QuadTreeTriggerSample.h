#pragma once

#include <iostream>
#include "Sample.h"
#include "Random.h"


class QuadTreeTriggerSample : public Sample, public ContactListener
{
private:
    std::vector<int> _triggerNbrPerCollider;

    static constexpr float SPEED = 200;
    static constexpr float RADIUS = Metrics::MetersToPixels(0.2f);

    static constexpr std::size_t CIRCLE_NBR = 5;

public:
    void BeginContact(ColliderRef &col1, ColliderRef &col2) noexcept override;

    void EndContact(ColliderRef &col1, ColliderRef &col2) noexcept override;

protected:
    void SampleSetUp() noexcept override;

    void SampleUpdate() noexcept override;

    void SampleTearDown() noexcept override;
};