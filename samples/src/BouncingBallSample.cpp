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

	body1.Position = { Metrics::Width / 2, Metrics::Height - Metrics::Height / 5 };

	auto colRef1 = _world.CreateCollider(bodyRef1);
	_colRefs.push_back(colRef1);
	auto& col1 = _world.GetCollider(colRef1);
	col1.Shape = Math::RectangleF({ -200,-100 }, { 200,100 });
	col1.BodyPosition = body1.Position;

	GraphicsData bd;
	bd.Shape = Math::RectangleF({ -200,-100 }, { 200,100 }) + body1.Position;
	AllGraphicsData.push_back(bd);

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
			/*if (i!=0)
			{
				_world.GetBody(col.BodyRef).ApplyForce({ 0,500 });
			}*/

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

	////Create Rectangle
	//auto bodyRef1 = _world.CreateBody();
	//_bodyRefs.push_back(bodyRef1);
	//auto& body1 = _world.GetBody(bodyRef1);
	//body1.type = BodyType::DYNAMIC;

	//body1.Position = { position };

	//auto colRef1 = _world.CreateCollider(bodyRef1);
	//_colRefs.push_back(colRef1);
	//auto& col1 = _world.GetCollider(colRef1);
	//col1.Shape = Math::RectangleF(Math::Vec2F::Zero(), {40,40});
	//col1.BodyPosition = body1.Position;

	//GraphicsData bd;
	//bd.Shape = Math::RectangleF(Math::Vec2F::Zero(), { 40,40 }) + body1.Position;
	//AllGraphicsData.push_back(bd);

	auto circleBodyRef = _world.CreateBody();
	_bodyRefs.push_back(circleBodyRef);
	auto& circleBody = _world.GetBody(circleBodyRef);

	circleBody.Mass = 1;

	circleBody.Position = position;

	auto circleColRef = _world.CreateCollider(circleBodyRef);
	_colRefs.push_back(circleColRef);
	auto& circleCol = _world.GetCollider(circleColRef);
	circleCol.Shape = Math::Circle(Math::Vec2F::Zero(), CIRCLE_RADIUS);
	circleCol.BodyPosition = circleBody.Position;
	circleCol.Restitution = 0.5f;

	GraphicsData circleData;
	circleData.Shape = Math::Circle(Math::Vec2F::Zero(), CIRCLE_RADIUS) + circleBody.Position;
	AllGraphicsData.push_back(circleData);
}
