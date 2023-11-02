#pragma once

#include "Sample.h"
#include "Random.h"

class FormsTriggerSample : public Sample, public ContactListener
{
private:
    std::vector<int> _triggerNbrPerCollider;

    BodyRef _starBodyRef;

public:
    void BeginContact(ColliderRef col1, ColliderRef col2) noexcept override;

    void EndContact(ColliderRef col1, ColliderRef col2) noexcept override;

protected:
    void SampleSetUp() noexcept override;

    void SampleUpdate() noexcept override;

    void SampleTearDown() noexcept override;
};