#include "SampleManager.h"

void SampleManager::SetUp() noexcept
{
    _samples.push_back(UniquePtr<DefaultSample>(new DefaultSample()));
    _samples.push_back(UniquePtr<StarSystemSample>(new StarSystemSample()));
    _samples.push_back(UniquePtr<FormsTriggerSample>(new FormsTriggerSample()));
    _samples.push_back(UniquePtr<TriggerSample>(new TriggerSample()));
    _samples.push_back(UniquePtr<BouncingCollisionSample>(new BouncingCollisionSample()));
    _samples.push_back(UniquePtr<GroundCollisionSample>(new GroundCollisionSample()));


    _samples[_sampleIdx]->SetUp();
}

void SampleManager::UpdateSample() const noexcept
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
        _sampleIdx = 1;
    else
        _sampleIdx++;
    _samples[_sampleIdx]->SetUp();
}

void SampleManager::PreviousSample() noexcept
{
    _samples[_sampleIdx]->TearDown();
    if (_sampleIdx <= 1)
        _sampleIdx = _samples.size() - 1;
    else
        _sampleIdx--;
    _samples[_sampleIdx]->SetUp();
}

void SampleManager::RegenerateSample() const noexcept
{
    _samples[_sampleIdx]->TearDown();
    _samples[_sampleIdx]->SetUp();
}

std::vector<GraphicsData> &SampleManager::GetSampleData() const noexcept
{
    return _samples[_sampleIdx]->AllGraphicsData;
}

void SampleManager::GiveMousePositionToSample(const Math::Vec2F mousePosition) const noexcept
{
    _samples[_sampleIdx]->GetMousePos(mousePosition);
}

void SampleManager::GiveLeftMouseClickToSample() const noexcept
{
    _samples[_sampleIdx]->OnLeftClick();
}

void SampleManager::GiveRightMouseClickToSample() const noexcept
{
    _samples[_sampleIdx]->OnRightClick();
}
