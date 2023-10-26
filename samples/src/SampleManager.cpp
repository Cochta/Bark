#include "SampleManager.h"

void SampleManager::SetUp() noexcept
{
    samples.push_back(UniquePtr<StarSystemSample>(new StarSystemSample()));
    samples.push_back(UniquePtr<QuadTreeTriggerSample>(new QuadTreeTriggerSample()));
    samples.push_back(UniquePtr<FormsTriggerSample>(new FormsTriggerSample()));

    samples[_sampleIdx]->SetUp();
}

void SampleManager::UpdateSample() noexcept
{
    samples[_sampleIdx]->Update();
}

void SampleManager::NextSample() noexcept
{
    samples[_sampleIdx]->TearDown();
    if (_sampleIdx >= samples.size() - 1)
        _sampleIdx = 0;
    else
        _sampleIdx++;
    samples[_sampleIdx]->SetUp();
}

void SampleManager::PreviousSample() noexcept
{
    samples[_sampleIdx]->TearDown();
    if (_sampleIdx <= 0)
        _sampleIdx = samples.size() - 1;
    else
        _sampleIdx--;
    samples[_sampleIdx]->SetUp();
}

void SampleManager::RegenerateSample() noexcept
{
    samples[_sampleIdx]->TearDown();
    samples[_sampleIdx]->SetUp();
}

std::vector<GraphicsData> &SampleManager::GetSampleData() noexcept
{
    return samples[_sampleIdx]->AllGraphicsData;
}

void SampleManager::GiveMousePositionToSample(const Math::Vec2F mousePosition) noexcept
{
    samples[_sampleIdx]->GetMousePos(mousePosition);
}
