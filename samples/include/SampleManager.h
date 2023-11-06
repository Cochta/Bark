#pragma once

#include "DefaultSample.h"
#include "StarSystemSample.h"
#include "FormsTriggerSample.h"
#include "QuadTreeTriggerSample.h"
#include "CollisionSample.h"
#include "BouncingBallSample.h"


#include "UniquePtr.h"

class SampleManager
{
private:
	std::vector<UniquePtr<Sample>> _samples;

	std::size_t _sampleIdx = 0;

public:
	[[nodiscard]] int GetSampleNbr() noexcept { return _samples.size(); }
	[[nodiscard]] int GetCurrentIndex() noexcept { return _sampleIdx; }

	[[nodiscard]] std::string GetSampleName(int idx) { return _samples[idx].Get()->GetName(); }
	[[nodiscard]] std::string GetSampleDescription(int idx) { return _samples[idx].Get()->GetDescription(); }

	void SetUp() noexcept;

	void UpdateSample() noexcept;

	void ChangeSample(int idx) noexcept;

	void NextSample() noexcept;

	void PreviousSample() noexcept;

	void RegenerateSample() noexcept;

	[[nodiscard]] std::vector<GraphicsData>& GetSampleData() noexcept;

	void GiveMousePositionToSample(Math::Vec2F mousePosition) noexcept;

	void GiveMouseClickToSample() noexcept;
};