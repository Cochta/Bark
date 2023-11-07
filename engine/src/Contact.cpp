#include "Contact.h"

void Contact::Resolve()
{
	const auto delta = CollidingBodies[0].body->Position - CollidingBodies[1].body->Position;

	switch (CollidingBodies[0].collider->Shape.index())
	{
	case static_cast<int>(Math::ShapeType::Circle):
		switch (CollidingBodies[1].collider->Shape.index())
		{
		case static_cast<int>(Math::ShapeType::Circle):
		{
			Normal = delta.Normalized();
			Penetration = std::get<Math::CircleF>(CollidingBodies[0].collider->Shape).Radius() +
				std::get<Math::CircleF>(CollidingBodies[1].collider->Shape).Radius() - delta.Length();
		}
		break;
		case static_cast<int>(Math::ShapeType::Rectangle):
		{
			const Math::CircleF& circle = std::get<Math::CircleF>(CollidingBodies[0].collider->Shape);
			const Math::RectangleF& rectangle = std::get<Math::RectangleF>(CollidingBodies[1].collider->Shape);

			Math::Vec2F closest(
				Math::Clamp(CollidingBodies[0].body->Position.X, rectangle.MinBound().X + CollidingBodies[1].body->Position.X, rectangle.MaxBound().X + CollidingBodies[1].body->Position.X),
				Math::Clamp(CollidingBodies[0].body->Position.Y, rectangle.MinBound().Y + CollidingBodies[1].body->Position.Y, rectangle.MaxBound().Y + CollidingBodies[1].body->Position.Y));

			Math::Vec2F delta = closest - CollidingBodies[0].body->Position;

			float distance = delta.Length();

			Penetration = circle.Radius() - distance;

			if (distance >= Math::Epsilon)
			{
				Normal = -delta.Normalized();
			}

		}
		break;
		}
		break;
	case static_cast<int>(Math::ShapeType::Rectangle):
		switch (CollidingBodies[1].collider->Shape.index())
		{
		case static_cast<int>(Math::ShapeType::Circle):
		{
			std::swap(CollidingBodies[0], CollidingBodies[1]);
			Resolve();
		}
		break;
		case static_cast<int>(Math::ShapeType::Rectangle):
		{
			const Math::Vec2F penetration(
				std::get<Math::RectangleF>(CollidingBodies[0].collider->Shape).HalfSize() +
				std::get<Math::RectangleF>(CollidingBodies[1].collider->Shape).HalfSize() -
				Math::Vec2F(std::abs(delta.X), std::abs(delta.Y))
			);

			if (penetration.X < penetration.Y)
			{
				Normal = delta.X > 0 ? Math::Vec2F(1.0f, 0.0f) : Math::Vec2F(-1.0f, 0.0f);

				Penetration = penetration.Normalized().X;
			}
			else
			{
				Normal = delta.Y > 0 ? Math::Vec2F(0.f, 1.0f) : Math::Vec2F(0.f, -1.0f);

				Penetration = penetration.Normalized().Y;
			}
		}
		break;
		}
	}

	const auto mass1 = CollidingBodies[0].body->Mass, mass2 = CollidingBodies[1].body->Mass;
	const auto rest1 = CollidingBodies[0].collider->Restitution, rest2 = CollidingBodies[1].collider->Restitution;

	Restitution = (mass1 * rest1 + mass2 * rest2) / (mass1 + mass2);

	ResolveVelocity();
	ResolveInterpenetration();
}

float Contact::CalculateSeparateVelocity() const noexcept
{
	const auto relativeVelocity = CollidingBodies[0].body->Velocity - CollidingBodies[1].body->Velocity;
	return relativeVelocity.Dot(Normal);
}

void Contact::ResolveVelocity() noexcept
{
	// Calculate the separating velocity.
	const float separatingVelocity = CalculateSeparateVelocity();

	if (separatingVelocity > 0) {
		return; // No collision resolution needed if separating.
	}

	// Calculate the new separating velocity with restitution.
	const float newSeparatingVelocity = -separatingVelocity * Restitution;

	// Calculate the delta velocity.
	const float deltaVelocity = newSeparatingVelocity - separatingVelocity;

	// Calculate inverse masses.
	const float inverseMass1 = 1 / CollidingBodies[0].body->Mass;
	const float inverseMass2 = 1 / CollidingBodies[1].body->Mass;
	const float totalInverseMass = inverseMass1 + inverseMass2;

	if (totalInverseMass <= 0) {
		return; // All particles have infinite mass, do nothing.
	}

	// Calculate the impulse.
	const float impulse = deltaVelocity / totalInverseMass;
	const auto impulsePerIMass = Normal * impulse;

	// Apply the impulse to the bodies based on their types.
	if (CollidingBodies[0].body->type == BodyType::DYNAMIC)
	{
		CollidingBodies[0].body->Velocity += impulsePerIMass * inverseMass1;
	}
	else if (CollidingBodies[0].body->type == BodyType::STATIC)
	{
		CollidingBodies[1].body->Velocity -= impulsePerIMass * inverseMass2;
	}

	if (CollidingBodies[1].body->type == BodyType::DYNAMIC)
	{
		CollidingBodies[1].body->Velocity -= impulsePerIMass * inverseMass2;
	}
	else if (CollidingBodies[1].body->type == BodyType::STATIC)
	{
		CollidingBodies[0].body->Velocity += impulsePerIMass * inverseMass2;
	}
}

void Contact::ResolveInterpenetration() noexcept
{
	// If we don't have any penetration, skip this step.
	if (Penetration <= 0) return;

	// Calculate inverse masses.
	const float inverseMass1 = 1 / CollidingBodies[0].body->Mass;
	const float inverseMass2 = 1 / CollidingBodies[1].body->Mass;
	const float totalInverseMass = inverseMass1 + inverseMass2;

	if (totalInverseMass <= 0) {
		return;
	}

	// Calculate the amount of penetration resolution per unit of inverse mass.
	const auto movePerIMass = Normal * (Penetration / totalInverseMass);

	// Apply the penetration resolution to dynamic bodies.
	if (CollidingBodies[0].body->type == BodyType::DYNAMIC)
	{
		CollidingBodies[0].body->Position += movePerIMass * inverseMass1;
	}
	if (CollidingBodies[1].body->type == BodyType::DYNAMIC)
	{
		CollidingBodies[1].body->Position -= movePerIMass * inverseMass2;
	}
}