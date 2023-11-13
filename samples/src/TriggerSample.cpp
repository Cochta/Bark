#include "TriggerSample.h"

std::string TriggerSample::GetName() noexcept
{
	return "Trigger";
}

std::string TriggerSample::GetDescription() noexcept
{
	return "Randomly generated objects, they become green if they detect a trigger otherwise they stay blue, the trigger detection uses a QuadTree. ";
}

void TriggerSample::OnTriggerEnter(ColliderRef col1, ColliderRef col2) noexcept
{
	_triggerNbrPerCollider[col1.Index]++;
	_triggerNbrPerCollider[col2.Index]++;
}

void TriggerSample::OnTriggerExit(ColliderRef col1, ColliderRef col2) noexcept
{
	_triggerNbrPerCollider[col1.Index]--;
	_triggerNbrPerCollider[col2.Index]--;
}

void TriggerSample::SampleSetUp() noexcept
{
	_world.SetContactListener(this);
	_nbObjects = CIRCLE_NBR + RECTANGLE_NBR + TRIANGLE_NBR;
	_triggerNbrPerCollider.resize(_nbObjects, 0);
	AllGraphicsData.reserve(_nbObjects);
	_bodyRefs.reserve(_nbObjects);
	_colRefs.reserve(_nbObjects);

	//Create Circles
	for (std::size_t i = 0; i < CIRCLE_NBR; ++i)
	{
		const auto circleBodyRef = _world.CreateBody();
		_bodyRefs.push_back(circleBodyRef);
		auto& circleBody = _world.GetBody(circleBodyRef);

		circleBody.Velocity = Math::Vec2F(Math::Random::Range(-1.f, 1.f),
			Math::Random::Range(-1.f, 1.f)) * SPEED;

		circleBody.Position = { Math::Random::Range(100.f, Metrics::Width - 100.f),
						  Math::Random::Range(100.f, Metrics::Height - 100.f) };

		const auto circleColRef = _world.CreateCollider(circleBodyRef);
		_colRefs.push_back(circleColRef);
		auto& circleCol = _world.GetCollider(circleColRef);
		circleCol.Shape = Math::Circle(Math::Vec2F::Zero(), CIRCLE_RADIUS);
		circleCol.BodyPosition = circleBody.Position;
		circleCol.IsTrigger = true;

		GraphicsData cgd;
		cgd.Shape = Math::Circle(Math::Vec2F::Zero(), CIRCLE_RADIUS) + circleBody.Position;
		AllGraphicsData.push_back(cgd);
	}

	//Create Rectangles
	for (std::size_t i = 0; i < RECTANGLE_NBR; ++i)
	{
		const auto rectBodyRef = _world.CreateBody();
		_bodyRefs.push_back(rectBodyRef);
		auto& rectBody = _world.GetBody(rectBodyRef);

		rectBody.Velocity = Math::Vec2F(Math::Random::Range(-1.f, 1.f),
			Math::Random::Range(-1.f, 1.f)) * SPEED;

		rectBody.Position = { Math::Random::Range(100.f, Metrics::Width - 100.f),
						  Math::Random::Range(100.f, Metrics::Height - 100.f) };

		const auto rectColRef = _world.CreateCollider(rectBodyRef);
		_colRefs.push_back(rectColRef);
		auto& rectCol = _world.GetCollider(rectColRef);
		rectCol.Shape = Math::RectangleF(Math::Vec2F::Zero(), RECTANGLE_BOUNDS);
		rectCol.BodyPosition = rectBody.Position;
		rectCol.IsTrigger = true;

		GraphicsData rbd;
		rbd.Shape = Math::RectangleF(Math::Vec2F::Zero(), RECTANGLE_BOUNDS) + rectBody.Position;
		AllGraphicsData.push_back(rbd);
	}
	//Create Triangles
	for (std::size_t i = 0; i < TRIANGLE_NBR; ++i)
	{
		const auto triangleBodyRef = _world.CreateBody();
		_bodyRefs.push_back(triangleBodyRef);
		auto& triangleBody = _world.GetBody(triangleBodyRef);

		triangleBody.Velocity = Math::Vec2F(Math::Random::Range(-1.f, 1.f),
			Math::Random::Range(-1.f, 1.f)) * SPEED;

		triangleBody.Position = { Math::Random::Range(100.f, Metrics::Width - 100.f),
						  Math::Random::Range(100.f, Metrics::Height - 100.f) };

		const auto triangleColRef = _world.CreateCollider(triangleBodyRef);
		_colRefs.push_back(triangleColRef);
		auto& triangleCol = _world.GetCollider(triangleColRef);
		triangleCol.Shape = Math::PolygonF(TRIANGLE_VERTICES);
		triangleCol.BodyPosition = triangleBody.Position;
		triangleCol.IsTrigger = true;

		GraphicsData tbd;
		tbd.Shape = Math::PolygonF(TRIANGLE_VERTICES) + triangleBody.Position;
		AllGraphicsData.push_back(tbd);
	}
}

void TriggerSample::DrawQuadtree(const QuadNode& node) noexcept
{
	if (node.Children[0] == nullptr)
	{
		_quadTreeGraphicsData.push_back({ Math::RectangleF(node.Bounds), false });
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			DrawQuadtree(*node.Children[i]);
		}
	}
}

void TriggerSample::SampleUpdate() noexcept
{
	if (_nbObjects < AllGraphicsData.size())
	{
		AllGraphicsData.erase(AllGraphicsData.begin() + _nbObjects, AllGraphicsData.end());
	}

	for (std::size_t i = 0; i < _colRefs.size(); ++i)
	{		
		auto& col = _world.GetCollider(_colRefs[i]);
		auto& bounds = col.GetBounds();
		auto& body = _world.GetBody(col.BodyRef);

		if (bounds.MinBound().X <= 0)
		{
			body.Velocity.X = Math::Abs(body.Velocity.X);
		}
		else if (bounds.MaxBound().X >= Metrics::Width)
		{
			body.Velocity.X = -Math::Abs(body.Velocity.X);
		}
		if (bounds.MinBound().Y <= 0)
		{
			body.Velocity.Y = Math::Abs(body.Velocity.Y);
		}
		else if (bounds.MaxBound().Y >= Metrics::Height)
		{
			body.Velocity.Y = -Math::Abs(body.Velocity.Y);
		}

		auto& shape = _world.GetCollider(_colRefs[i]).Shape;

		switch (shape.index())
		{
		case static_cast<int>(Math::ShapeType::Circle):
			AllGraphicsData[i].Shape = std::get<Math::CircleF>(shape) + body.Position;
			break;
		case static_cast<int>(Math::ShapeType::Rectangle):
			AllGraphicsData[i].Shape = std::get<Math::RectangleF>(shape) + body.Position;
			break;
		case static_cast<int>(Math::ShapeType::Polygon):
			AllGraphicsData[i].Shape = std::get<Math::PolygonF>(shape) + body.Position;
			break;
		}

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
	DrawQuadtree(_world._quadTree.Nodes[0]);
	AllGraphicsData.insert(AllGraphicsData.end(), _quadTreeGraphicsData.begin(), _quadTreeGraphicsData.end());
}

void TriggerSample::SampleTearDown() noexcept
{
	_triggerNbrPerCollider.clear();
	_quadTreeGraphicsData.clear();
}
