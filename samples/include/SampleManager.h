#pragma once

#include "StarSystemSample.h"
#include "FormsTriggerSample.h"
#include "QuadTreeTriggerSample.h"

#include "UniquePtr.h"

class SampleManager
{
private:
    std::vector<UniquePtr<Sample>> samples;

    std::size_t _sampleIdx = 0;

public:
    void SetUp() noexcept;

    void UpdateSample() noexcept;

    void NextSample() noexcept;

    void PreviousSample() noexcept;

    void RegenerateSample() noexcept;

    std::vector<GraphicsData> &GetSampleData() noexcept;

    void GiveMousePositionToSample(Math::Vec2F mousePosition) noexcept;
};