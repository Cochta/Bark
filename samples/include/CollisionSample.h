#pragma once

#include <iostream>
#include "Sample.h"
#include "Random.h"


class CollisionSample : public Sample, public ContactListener
{
private:
	std::vector<GraphicsData> _quadTreeGraphicsData;
	std::vector<int> _collisionNbrPerCollider;


	std::vector<Math::Vec2F> TRIANGLE_VERTICES = {
		{Metrics::MetersToPixels(0.0f),    Metrics::MetersToPixels(-0.2f)},
		{Metrics::MetersToPixels(0.2f),    Metrics::MetersToPixels(0.2f)},
		{Metrics::MetersToPixels(-0.2f),  Metrics::MetersToPixels(0.2f)},
	};
	static constexpr Math::Vec2F RECTANGLE_BOUNDS = { Metrics::MetersToPixels(0.6f), Metrics::MetersToPixels(0.6f) };
	static constexpr float CIRCLE_RADIUS = Metrics::MetersToPixels(0.3f);
	static constexpr float SPEED = 200;

	static constexpr std::size_t CIRCLE_NBR = 50;
	static constexpr std::size_t RECTANGLE_NBR = 50;
	static constexpr std::size_t TRIANGLE_NBR = 0;

	std::size_t _nbObjects;

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
	void DrawQuadtree(const QuadNode* node);

};