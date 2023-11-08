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
		col1.BodyPosition = body1.Position;
		col1.IsTrigger = true;

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
		col1.BodyPosition = body1.Position;
		col1.IsTrigger = true;

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
		col1.BodyPosition = body1.Position;
		col1.IsTrigger = true;

		GraphicsData bd;
		bd.Shape = Math::PolygonF(TRIANGLE_VERTICES) + body1.Position;
		AllGraphicsData.push_back(bd);
	}
}

void TriggerSample::DrawQuadtree(const QuadNode* node)
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
			AllGraphicsData[i].Shape = std::get<Math::CircleF>(shape) + col.BodyPosition;
			break;
		case static_cast<int>(Math::ShapeType::Rectangle):
			AllGraphicsData[i].Shape = std::get<Math::RectangleF>(shape) + col.BodyPosition;
			break;
		case static_cast<int>(Math::ShapeType::Polygon):
			AllGraphicsData[i].Shape = std::get<Math::PolygonF>(shape) + col.BodyPosition;
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

void TriggerSample::SampleTearDown() noexcept
{
	_triggerNbrPerCollider.clear();
	_quadTreeGraphicsData.clear();
}