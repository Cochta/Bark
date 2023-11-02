#include "QuadTreeTriggerSample.h"

void QuadTreeTriggerSample::BeginContact(ColliderRef col1, ColliderRef col2) noexcept
{
	_triggerNbrPerCollider[col1.Index]++;
	_triggerNbrPerCollider[col2.Index]++;
}

void QuadTreeTriggerSample::EndContact(ColliderRef col1, ColliderRef col2) noexcept
{
	_triggerNbrPerCollider[col1.Index]--;
	_triggerNbrPerCollider[col2.Index]--;
}

void QuadTreeTriggerSample::SampleSetUp() noexcept
{
	_world.SetContactListener(this);
	_triggerNbrPerCollider.resize(CIRCLE_NBR, 0);

	for (std::size_t i = 0; i < CIRCLE_NBR; ++i)
	{
		auto bodyRef1 = _world.CreateBody();
		_bodyRefs.push_back(bodyRef1);
		auto& body1 = _world.GetBody(bodyRef1);

		body1.Velocity = Math::Vec2F(Math::Random::Range(-1.f, 1.f),
									 Math::Random::Range(-1.f, 1.f)) * SPEED;

		body1.Position = { Math::Random::Range(100.f, Metrics::Width - 100.f),
						  Math::Random::Range(100.f, Metrics::Height - 100.f) };

		auto colRef1 = _world.CreateCollider(bodyRef1);
		_colRefs.push_back(colRef1);
		auto& col1 = _world.GetCollider(colRef1);
		col1.Shape = Math::Circle(Math::Vec2F::Zero(), RADIUS);

		GraphicsData bd;
		bd.Shape = Math::Circle(Math::Vec2F::Zero(), RADIUS) + body1.Position;
		AllGraphicsData.push_back(bd);
	}
}

void QuadTreeTriggerSample::DrawQuadtree(const QuadNode* node)
{
	if (node->Children[0] == nullptr)
	{
		_quadTreeGraphicsData.push_back({ Math::RectangleF(node->Bounds), false });
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			DrawQuadtree(node->Children[i]);
		}
	}
}

void QuadTreeTriggerSample::SampleUpdate() noexcept
{
	if (CIRCLE_NBR < AllGraphicsData.size())
	{
		AllGraphicsData.erase(AllGraphicsData.begin() + CIRCLE_NBR, AllGraphicsData.end());
	}

	for (std::size_t i = 0; i < _bodyRefs.size(); ++i)
	{
		auto& body = _world.GetBody(_bodyRefs[i]);
		if (body.Position.X - RADIUS <= 0)
		{
			body.Velocity.X = Math::Abs(body.Velocity.X);
		}
		else if (body.Position.X + RADIUS >= Metrics::Width)
		{
			body.Velocity.X = -Math::Abs(body.Velocity.X);
		}
		if (body.Position.Y - RADIUS <= 0)
		{
			body.Velocity.Y = Math::Abs(body.Velocity.Y);
		}
		else if (body.Position.Y + RADIUS >= Metrics::Height)
		{
			body.Velocity.Y = -Math::Abs(body.Velocity.Y);
		}

		AllGraphicsData[i].Shape = std::get<Math::CircleF>(_world.GetCollider(_colRefs[i]).Shape) + body.Position;
	}

	for (int i = 0; i < _colRefs.size(); ++i)
	{
		if (_triggerNbrPerCollider[i] > 0)
		{
			AllGraphicsData[i].Color = { 0, 255, 0, 255 };
		}
		else
		{
			AllGraphicsData[i].Color = { 0, 0, 255, 255 };
		}
	}

	_quadTreeGraphicsData.clear();
	DrawQuadtree(&_world._quadTree._root);
	AllGraphicsData.insert(AllGraphicsData.end(), _quadTreeGraphicsData.begin(), _quadTreeGraphicsData.end());

}

void QuadTreeTriggerSample::SampleTearDown() noexcept
{
	_triggerNbrPerCollider.clear();
}
