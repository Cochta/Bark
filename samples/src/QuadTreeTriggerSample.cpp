#include "QuadTreeTriggerSample.h"

std::string QuadTreeTriggerSample::GetName() noexcept
{
	return "Trigger";
}

std::string QuadTreeTriggerSample::GetDescription() noexcept
{
	return "Randomly generated objects, they become green if they detect a trigger otherwise they stay blue, the trigger detection uses a QuadTree. ";
}

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
	_nbObjects = CIRCLE_NBR + RECTANGLE_NBR + TRIANGLE_NBR;
	_triggerNbrPerCollider.resize(_nbObjects, 0);
	AllGraphicsData.reserve(_nbObjects);
	_bodyRefs.reserve(_nbObjects);
	_colRefs.reserve(_nbObjects);

	//Create Circles
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
		col1.Shape = Math::Circle(Math::Vec2F::Zero(), CIRCLE_RADIUS);

		GraphicsData bd;
		bd.Shape = Math::Circle(Math::Vec2F::Zero(), CIRCLE_RADIUS) + body1.Position;
		AllGraphicsData.push_back(bd);
	}

	//Create Rectangles
	for (std::size_t i = 0; i < RECTANGLE_NBR; ++i)
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
		col1.Shape = Math::RectangleF(Math::Vec2F::Zero(), RECTANGLE_BOUNDS);

		GraphicsData bd;
		bd.Shape = Math::RectangleF(Math::Vec2F::Zero(), RECTANGLE_BOUNDS) + body1.Position;
		AllGraphicsData.push_back(bd);
	}
	//Create Triangles
	for (std::size_t i = 0; i < TRIANGLE_NBR; ++i)
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
		col1.Shape = Math::PolygonF(TRIANGLE_VERTICES);

		GraphicsData bd;
		bd.Shape = Math::PolygonF(TRIANGLE_VERTICES) + body1.Position;
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
			DrawQuadtree(node->Children[i].get());
		}
	}
}

void QuadTreeTriggerSample::SampleUpdate() noexcept
{
	if (_nbObjects < AllGraphicsData.size())
	{
		AllGraphicsData.erase(AllGraphicsData.begin() + _nbObjects, AllGraphicsData.end());
	}

	for (std::size_t i = 0; i < _bodyRefs.size(); ++i)
	{
		auto& body = _world.GetBody(_bodyRefs[i]);
		if (body.Position.X - CIRCLE_RADIUS <= 0)
		{
			body.Velocity.X = Math::Abs(body.Velocity.X);
		}
		else if (body.Position.X + CIRCLE_RADIUS >= Metrics::Width)
		{
			body.Velocity.X = -Math::Abs(body.Velocity.X);
		}
		if (body.Position.Y - CIRCLE_RADIUS <= 0)
		{
			body.Velocity.Y = Math::Abs(body.Velocity.Y);
		}
		else if (body.Position.Y + CIRCLE_RADIUS >= Metrics::Height)
		{
			body.Velocity.Y = -Math::Abs(body.Velocity.Y);
		}

		auto shape = _world.GetCollider(_colRefs[i]).Shape;

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
	DrawQuadtree(&_world._quadTree);
	AllGraphicsData.insert(AllGraphicsData.end(), _quadTreeGraphicsData.begin(), _quadTreeGraphicsData.end());
}

void QuadTreeTriggerSample::SampleTearDown() noexcept
{
	_triggerNbrPerCollider.clear();
	_quadTreeGraphicsData.clear();
}
