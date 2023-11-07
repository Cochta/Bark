#pragma once

#include "Collider.h"
#include "Body.h"

/**
 * @class ContactListener
 * @brief An abstract class for handling collision events.
 */
class ContactListener
{
public:
	/**
	 * @brief Called when a trigger begins.
	 * @param col1 The first collider ref involved in the collision.
	 * @param col2 The second collider ref involved in the collision.
	 */
	virtual void OnTriggerEnter(ColliderRef colRef1, ColliderRef colRef2) noexcept = 0;

	/**
	 * @brief Called when a trigger ends.
	 * @param col1 The first collider ref involved in the collision.
	 * @param col2 The second collider ref involved in the collision.
	 */
	virtual void OnTriggerExit(ColliderRef colRef1, ColliderRef colRef2) noexcept = 0;

	/**
	 * @brief Called when a collision begins.
	 * @param col1 The first collider involved in the collision.
	 * @param col2 The second collider involved in the collision.
	 */
	virtual void OnCollisionEnter(ColliderRef colRef1, ColliderRef colRef2) noexcept = 0;

	/**
	 * @brief Called when a collision ends.
	 * @param col1 The first collider involved in the collision.
	 * @param col2 The second collider involved in the collision.
	 */
	virtual void OnCollisionExit(ColliderRef colRef1, ColliderRef colRef2) noexcept = 0;
};

struct CollidingBody
{
	Body* body = nullptr;
	Collider* collider = nullptr;
};

class Contact
{
public:
	std::array<CollidingBody, 2> CollidingBodies{};
	Math::Vec2F Normal{};
	//Math::Vec2F Position{};
	float Restitution{ 1 };
	float Penetration{};
	void Resolve();
	float CalculateSeparateVelocity() const noexcept;
	void ResolveVelocity() noexcept;
	void ResolveInterpenetration() noexcept;

};