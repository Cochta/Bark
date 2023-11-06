#pragma once

#include <iostream>
#include "Sample.h"
#include "Random.h"


class BouncingBallSample : public Sample, public ContactListener
{
private:

	static constexpr Math::Vec2F RECTANGLE_BOUNDS = { Metrics::Width / 3, Metrics::Height / 5 };
	static constexpr float CIRCLE_RADIUS = Metrics::MetersToPixels(0.3f);
	static constexpr float SPEED = 200;

public:
	std::string GetName() noexcept override;
	std::string GetDescription() noexcept override;

	void OnTriggerEnter(ColliderRef col1, ColliderRef col2) noexcept override {};

	void OnTriggerExit(ColliderRef col1, ColliderRef col2) noexcept override {};

	void OnCollisionEnter(ColliderRef col1, ColliderRef col2) noexcept override;

	void OnCollisionExit(ColliderRef col1, ColliderRef col2) noexcept override;

protected:
	void SampleSetUp() noexcept override;

	void SampleUpdate() noexcept override;

	void SampleTearDown() noexcept override;

private:
	void CreateBall(Math::Vec2F position) noexcept;

};