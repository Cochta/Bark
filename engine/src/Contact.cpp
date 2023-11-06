#include "Contact.h"

void Contact::Resolve()
{
	const auto delta = CollidingBodies[0].body->Position - CollidingBodies[1].body->Position;
	Normal = delta.Normalized();
	Position = CollidingBodies[0].body->Position + delta / 2;

	switch (CollidingBodies[0].collider->Shape.index())
	{
	case static_cast<int>(Math::ShapeType::Circle):
		switch (CollidingBodies[1].collider->Shape.index())
		{
		case static_cast<int>(Math::ShapeType::Circle):
		{
			Penetration = std::get<Math::CircleF>(CollidingBodies[0].collider->Shape).Radius() + std::get<Math::CircleF>(CollidingBodies[1].collider->Shape).Radius() - delta.Length();
		}
		break;
		case static_cast<int>(Math::ShapeType::Rectangle):
		{
			const Math::CircleF& circle = std::get<Math::CircleF>(CollidingBodies[0].collider->Shape);
			const Math::RectangleF& rectangle = std::get<Math::RectangleF>(CollidingBodies[1].collider->Shape);

			Math::Vec2F closest(
				Math::Clamp(CollidingBodies[0].body->Position.X, rectangle.MinBound().X, rectangle.MaxBound().X),
				Math::Clamp(CollidingBodies[0].body->Position.Y, rectangle.MinBound().Y, rectangle.MaxBound().Y));

			Math::Vec2F circleToClosest(CollidingBodies[0].body->Position - closest);

			float distance = circleToClosest.Length();

			if (distance < circle.Radius())
			{
				Penetration = circle.Radius() - distance;

				Normal = circleToClosest / distance;

				Position = CollidingBodies[0].body->Position + Normal * (circle.Radius() - Penetration);
			}
			else
			{
				Penetration = 0;
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
			const Math::Vec2F penetration = std::get<Math::RectangleF>(CollidingBodies[0].collider->Shape).HalfSize() + std::get<Math::RectangleF>(CollidingBodies[1].collider->Shape).HalfSize() - Math::Vec2F(std::abs(delta.X), std::abs(delta.Y));
			Penetration = std::min(penetration.X, penetration.Y);
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

void Contact::ResolveVelocity() const noexcept
{
	const auto separatingVelocity = CalculateSeparateVelocity();
	if (separatingVelocity > 0)
	{
		return;
	}
	const auto newSepVelocity = -separatingVelocity * Restitution;
	const auto deltaVelocity = newSepVelocity - separatingVelocity;

	const auto inverseMass1 = 1 / CollidingBodies[0].body->Mass;
	const auto inverseMass2 = 1 / CollidingBodies[1].body->Mass;

	const auto totalInverseMass = inverseMass1 + inverseMass2;
	const auto impulse = deltaVelocity / totalInverseMass;
	const auto impulsePerIMass = Normal * impulse;

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

void Contact::ResolveInterpenetration() const noexcept
{
	// If we don't have any penetration, skip this step.
	if (Penetration <= 0) return;

	const auto inverseMass1 = 1 / CollidingBodies[0].body->Mass;
	const auto inverseMass2 = 1 / CollidingBodies[1].body->Mass;
	const auto totalInverseMass = inverseMass1 + inverseMass2;

	// If all particles have infinite mass, then we do nothing.
	if (totalInverseMass <= 0) return;

	// Find the amount of penetration resolution per unit of inverse mass.
	const auto movePerIMass = Normal * (-Penetration / totalInverseMass);

	// Apply the penetration resolution.
	if (CollidingBodies[0].body->type == BodyType::DYNAMIC)
	{
		CollidingBodies[0].body->Position += movePerIMass * inverseMass1;
	}
	if (CollidingBodies[1].body->type == BodyType::DYNAMIC)
	{
		CollidingBodies[1].body->Position += movePerIMass * inverseMass2;
	}
}