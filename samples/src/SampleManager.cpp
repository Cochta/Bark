#include "SampleManager.h"

void SampleManager::SetUp() noexcept
{
    _samples.push_back(UniquePtr<DefaultSample>(new DefaultSample()));
    _samples.push_back(UniquePtr<StarSystemSample>(new StarSystemSample()));
    _samples.push_back(UniquePtr<FormsTriggerSample>(new FormsTriggerSample()));
    _samples.push_back(UniquePtr<QuadTreeTriggerSample>(new QuadTreeTriggerSample()));


    _samples[_sampleIdx]->SetUp();
}

void SampleManager::UpdateSample() noexcept
{
    _samples[_sampleIdx]->Update();
}

void SampleManager::ChangeSample(int idx) noexcept
{
    _samples[_sampleIdx]->TearDown();
    _sampleIdx = idx;
    _samples[_sampleIdx]->SetUp();
}

void SampleManager::NextSample() noexcept
{
    _samples[_sampleIdx]->TearDown();
    if (_sampleIdx >= _samples.size() - 1)
        _sampleIdx = 0;
    else
        _sampleIdx++;
    _samples[_sampleIdx]->SetUp();
}

void SampleManager::PreviousSample() noexcept
{
    _samples[_sampleIdx]->TearDown();
    if (_sampleIdx <= 0)
        _sampleIdx = _samples.size() - 1;
    else
        _sampleIdx--;
    _samples[_sampleIdx]->SetUp();
}

void SampleManager::RegenerateSample() noexcept
{
    _samples[_sampleIdx]->TearDown();
    _samples[_sampleIdx]->SetUp();
}

std::vector<GraphicsData> &SampleManager::GetSampleData() noexcept
{
    return _samples[_sampleIdx]->AllGraphicsData;
}

void SampleManager::GiveMousePositionToSample(const Math::Vec2F mousePosition) noexcept
{
    _samples[_sampleIdx]->GetMousePos(mousePosition);
}
