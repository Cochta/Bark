#include "GroundCollisionSample.h"

std::string GroundCollisionSample::GetName() noexcept
{
	return "Bouncing Forms";
}

std::string GroundCollisionSample::GetDescription() noexcept
{
	return "CONTROLS: Left click to create a circle, right click to create a rectangle.\nFloor has 1 x bounciness, Circles and rectangles have 0 x bounciness";
}

void GroundCollisionSample::OnCollisionEnter(ColliderRef col1, ColliderRef col2) noexcept
{

}

void GroundCollisionSample::OnCollisionExit(ColliderRef col1, ColliderRef col2) noexcept
{

}

void GroundCollisionSample::SampleSetUp() noexcept
{
	_world.SetContactListener(this);

	//Create static rectangle
	const auto groundRef = _world.CreateBody();
	_bodyRefs.push_back(groundRef);
	auto& groundBody = _world.GetBody(groundRef);
	groundBody.Type = BodyType::STATIC;

	groundBody.Position = { Metrics::Width / 2.f, Metrics::Height - Metrics::Height / 5.f };

	const auto groundColRef = _world.CreateCollider(groundRef);
	_colRefs.push_back(groundColRef);
	auto& groundCol = _world.GetCollider(groundColRef);
	groundCol.Shape = Math::RectangleF({ -Metrics::Width / 3.f, 0.f }, { Metrics::Width / 3.f, 10.f });
	groundCol.BodyPosition = groundBody.Position;
	groundCol.Restitution = 1.f;

	AllGraphicsData.emplace_back();
}
void GroundCollisionSample::SampleUpdate() noexcept
{
	if (_mouseLeftReleased)
	{
		CreateBall(_mousePos);
	}
	else if (_mouseRightReleased)
	{
		CreateRect(_mousePos);
	}

	for (std::size_t i = 0; i < _colRefs.size(); ++i)
	{
		const auto& col = _world.GetCollider(_colRefs[i]);

		const auto& shape = _world.GetCollider(_colRefs[i]).Shape;

		switch (shape.index())
		{
		case static_cast<int>(Math::ShapeType::Circle):
		_world.GetBody(col.BodyRef).ApplyForce({ 0,500 });
		AllGraphicsData[i].Shape = std::get<Math::CircleF>(shape) + col.BodyPosition;
		break;
		case static_cast<int>(Math::ShapeType::Rectangle):
		if (i != 0)
		{
			_world.GetBody(col.BodyRef).ApplyForce({ 0,500 });
		}
		AllGraphicsData[i].Shape = std::get<Math::RectangleF>(shape) + col.BodyPosition;
		break;
		default:
		break;
		}
	}
}

void GroundCollisionSample::SampleTearDown() noexcept
{

}

void GroundCollisionSample::CreateBall(Math::Vec2F position) noexcept
{
	const auto circleBodyRef = _world.CreateBody();
	_bodyRefs.push_back(circleBodyRef);
	auto& circleBody = _world.GetBody(circleBodyRef);

	circleBody.Mass = 1;

	circleBody.Position = position;

	const auto circleColRef = _world.CreateCollider(circleBodyRef);
	_colRefs.push_back(circleColRef);
	auto& circleCol = _world.GetCollider(circleColRef);
	circleCol.Shape = Math::Circle(Math::Vec2F::Zero(), Math::Random::Range(10.f, 40.f));
	circleCol.BodyPosition = circleBody.Position;
	circleCol.Restitution = 0.f;

	GraphicsData gd;
	gd.Color = {
	Math::Random::Range(0, 255),
	Math::Random::Range(0, 255),
	Math::Random::Range(0, 255),
	255 };


	AllGraphicsData.emplace_back(gd);
}

void GroundCollisionSample::CreateRect(Math::Vec2F position) noexcept
{
	const auto rectBodyRef = _world.CreateBody();
	_bodyRefs.push_back(rectBodyRef);
	auto& rectBody = _world.GetBody(rectBodyRef);
	rectBody.Type = BodyType::DYNAMIC;

	rectBody.Position = { position };

	const auto rectColRef = _world.CreateCollider(rectBodyRef);
	_colRefs.push_back(rectColRef);
	auto& rectCol = _world.GetCollider(rectColRef);
	rectCol.Shape = Math::RectangleF(
		{ -Math::Random::Range(1.f,4.f) * 10.f,-Math::Random::Range(1.f,4.f) * 10.f },
		{ Math::Random::Range(1.f,4.f) * 10.f,Math::Random::Range(1.f,4.f) * 10.f }
	);
	rectCol.BodyPosition = rectBody.Position;
	rectCol.Restitution = 0.f;

	GraphicsData gd;
	gd.Color = {
	Math::Random::Range(0, 255),
	Math::Random::Range(0, 255),
	Math::Random::Range(0, 255),
	255 };


	AllGraphicsData.emplace_back(gd);

}
