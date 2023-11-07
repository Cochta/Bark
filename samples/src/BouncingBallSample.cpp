#include "BouncingBallSample.h"

std::string BouncingBallSample::GetName() noexcept
{
	return "Bouncing ball";
}

std::string BouncingBallSample::GetDescription() noexcept
{
	return "The ball bounces on the floor";
}

void BouncingBallSample::OnCollisionEnter(ColliderRef col1, ColliderRef col2) noexcept
{
	//		AllGraphicsData[col1.Index].Color = {
	//Math::Random::Range(0, 255),
	//	Math::Random::Range(0, 255),
	//	Math::Random::Range(0, 255),
	//	255 };
	//	AllGraphicsData[col2.Index].Color = {
	//		Math::Random::Range(0, 255),
	//			Math::Random::Range(0, 255),
	//			Math::Random::Range(0, 255),
	//			255 };
}

void BouncingBallSample::OnCollisionExit(ColliderRef col1, ColliderRef col2) noexcept
{

}

void BouncingBallSample::SampleSetUp() noexcept
{
	_world.SetContactListener(this);

	//Create Rectangle
	auto bodyRef1 = _world.CreateBody();
	_bodyRefs.push_back(bodyRef1);
	auto& body1 = _world.GetBody(bodyRef1);
	body1.type = BodyType::STATIC;
	//body1.Mass = 1000;

	body1.Position = { Metrics::Width / 2, Metrics::Height - Metrics::Height / 5 };

	auto colRef1 = _world.CreateCollider(bodyRef1);
	_colRefs.push_back(colRef1);
	auto& col1 = _world.GetCollider(colRef1);
	col1.Shape = Math::RectangleF({ -Metrics::Width / 3, 0 }, { Metrics::Width / 3, 10 });
	col1.BodyPosition = body1.Position;
	col1.Restitution = 1.f;

	AllGraphicsData.emplace_back();

	//CreateBall({ Metrics::Width / 2, Metrics::Height / 5 });
}
void BouncingBallSample::SampleUpdate() noexcept
{
	if (_wasMouseReleased)
	{
		CreateBall(_mousePos);
	}
	for (std::size_t i = 0; i < _colRefs.size(); ++i)
	{
		auto& col = _world.GetCollider(_colRefs[i]);

		auto& shape = _world.GetCollider(_colRefs[i]).Shape;

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
		}
	}
}

void BouncingBallSample::SampleTearDown() noexcept
{

}

void BouncingBallSample::CreateBall(Math::Vec2F position) noexcept
{

	//Create Rectangle
	if (WhichForm)
	{
		auto bodyRef1 = _world.CreateBody();
		_bodyRefs.push_back(bodyRef1);
		auto& body1 = _world.GetBody(bodyRef1);
		body1.type = BodyType::DYNAMIC;

		body1.Position = { position };

		auto colRef1 = _world.CreateCollider(bodyRef1);
		_colRefs.push_back(colRef1);
		auto& col1 = _world.GetCollider(colRef1);
		col1.Shape = Math::RectangleF(
			{ -Math::Random::Range(1.f,4.f) * 10.f,-Math::Random::Range(1.f,4.f) * 10.f },
			{ Math::Random::Range(1.f,4.f) * 10.f,Math::Random::Range(1.f,4.f) * 10.f }
		);
		col1.BodyPosition = body1.Position;
		col1.Restitution = 0.f;
	}
	else
	{
		auto circleBodyRef = _world.CreateBody();
		_bodyRefs.push_back(circleBodyRef);
		auto& circleBody = _world.GetBody(circleBodyRef);

		circleBody.Mass = 1;

		circleBody.Position = position;

		auto circleColRef = _world.CreateCollider(circleBodyRef);
		_colRefs.push_back(circleColRef);
		auto& circleCol = _world.GetCollider(circleColRef);
		circleCol.Shape = Math::Circle(Math::Vec2F::Zero(), Math::Random::Range(10.f, 40.f));
		circleCol.BodyPosition = circleBody.Position;
		circleCol.Restitution = 0.f;
	}
	AllGraphicsData.emplace_back();
	WhichForm = !WhichForm;
}
