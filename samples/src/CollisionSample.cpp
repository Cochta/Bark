#include "CollisionSample.h"

std::string CollisionSample::GetName() noexcept
{
	return "Collision";
}

std::string CollisionSample::GetDescription() noexcept
{
	return "Randomly generated objects, they become green when they detect a physical collision otherwise they stay blue, the collision detection uses a QuadTree. ";
}

void CollisionSample::OnCollisionEnter(ColliderRef col1, ColliderRef col2) noexcept
{
	AllGraphicsData[col1.Index].Color = {
Math::Random::Range(0, 255),
	Math::Random::Range(0, 255),
	Math::Random::Range(0, 255),
	255 };
	AllGraphicsData[col2.Index].Color = {
		Math::Random::Range(0, 255),
			Math::Random::Range(0, 255),
			Math::Random::Range(0, 255),
			255 };
}

void CollisionSample::OnCollisionExit(ColliderRef col1, ColliderRef col2) noexcept
{

}

void CollisionSample::SampleSetUp() noexcept
{
	_world.SetContactListener(this);
	_nbObjects = CIRCLE_NBR + RECTANGLE_NBR + TRIANGLE_NBR;
	_collisionNbrPerCollider.resize(_nbObjects, 0);
	AllGraphicsData.reserve(_nbObjects);
	_bodyRefs.reserve(_nbObjects);
	_colRefs.reserve(_nbObjects);

	//Create Circles
	for (std::size_t i = 0; i < CIRCLE_NBR; ++i)
	{
		auto bodyRef1 = _world.CreateBody();
		_bodyRefs.push_back(bodyRef1);
		auto& body1 = _world.GetBody(bodyRef1);

		body1.Mass = 1;

		body1.Velocity = Math::Vec2F(Math::Random::Range(-1.f, 1.f),
			Math::Random::Range(-1.f, 1.f)) * SPEED;


		body1.Position = { Math::Random::Range(100.f, Metrics::Width - 100.f),
						  Math::Random::Range(100.f, Metrics::Height - 100.f) };

		//if (i % 2 == 0)
		//{
		//	body1.Mass = 1;

		//	body1.Velocity = Math::Vec2F(100, 0);


		//	body1.Position = { 200.f*i, 200.f };
		//}
		//else
		//{
		//	body1.Mass = 1;

		//	body1.Velocity = Math::Vec2F(0, 0);


		//	body1.Position = { 400.f*i, 200.f };
		//}

		auto colRef1 = _world.CreateCollider(bodyRef1);
		_colRefs.push_back(colRef1);
		auto& col1 = _world.GetCollider(colRef1);
		col1.Shape = Math::Circle(Math::Vec2F::Zero(), CIRCLE_RADIUS);
		col1.BodyPosition = body1.Position;

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

		GraphicsData bd;
		bd.Shape = Math::PolygonF(TRIANGLE_VERTICES) + body1.Position;
		AllGraphicsData.push_back(bd);
	}
}

void CollisionSample::DrawQuadtree(const QuadNode* node)
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

void CollisionSample::SampleUpdate() noexcept
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
	//float accumalated = 0;
	//for each (auto ref in _bodyRefs)
	//{
	//	accumalated += _world.GetBody(ref).Velocity.Length();
	//	
	//}
	//printf("%f\n", accumalated);

	_quadTreeGraphicsData.clear();
	DrawQuadtree(&_world._quadTree);
	AllGraphicsData.insert(AllGraphicsData.end(), _quadTreeGraphicsData.begin(), _quadTreeGraphicsData.end());
}

void CollisionSample::SampleTearDown() noexcept
{
	_collisionNbrPerCollider.clear();
	_quadTreeGraphicsData.clear();
}
