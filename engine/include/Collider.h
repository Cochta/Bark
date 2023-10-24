#pragma once

#include "Shape.h"
#include "Refs.h"

/**
 * @file Collider.h
 * @brief This file defines the `Shape`, `Collider`, `ColliderPair`, and `ContactListener` structures
 * and a `ColliderPairHash` class for managing physics-related data and collision events in a simulation.
 */

/**
 * @struct Shape
 * @brief Represents the shape associated with a collider.
 */
struct Shape
{
    Math::ShapeType Type; /**< The type of shape (Circle, Rectangle, or Polygon). */
    union //todo: check std::variant
    {
        Math::CircleF *Circle; /**< Pointer to a Circle shape. */
        Math::RectangleF *Rectangle; /**< Pointer to a Rectangle shape. */
        Math::PolygonF *Polygon; /**< Pointer to a Polygon shape. */
    };
};

/**
 * @class Collider
 * @brief Represents a collider in a physics simulation.
 */
class Collider
{
public:
    Shape *ColShape = nullptr; /**< The shape associated with the collider. */
    //float bounciness = 1.f;
    //float friction = 1.f;
    bool IsSensor = true; /**< Flag indicating if the collider is a sensor (non-physical). */
    bool IsAttached = false; /**< Flag indicating if the collider is attached to a body. */
    BodyRef BodyRef; /**< Reference to the body associated with the collider. */
    int ID; /**< Unique identifier for the collider. */

private:
    //todo: if no body ref then collider is not attached
    std::size_t _triggerNbr = 0; /**< Counter for tracking the number of triggers.*/

public:

    /**
     * @brief Checks if the collider is a trigger.
     * @return `true` if the collider is a trigger, `false` otherwise.
     */
    [[nodiscard]] bool DoesTrigger() const noexcept
    { return _triggerNbr > 0; }

    /**
     * @brief Called when a collision begins with the collider.
     */
    void OnTriggerEnter() noexcept
    {
        _triggerNbr++;
    }

    /**
     * @brief Called when a collision ends with the collider.
     */
    void OnTriggerExit() noexcept
    {
        _triggerNbr--;
    }

    /**
     * @brief Custom hash function for colliders.
     * @param collider The collider to hash.
     * @return The hash value of the collider's ID.
     */
    std::size_t operator()(const Collider &collider) const
    {
        return std::hash<int>{}(collider.ID);
    }
};

/**
 * @struct ColliderPair
 * @brief Represents a pair of colliders involved in a collision.
 */
struct ColliderPair
{
    Collider ColA; /**< The first collider in the pair. */
    Collider ColB; /**< The second collider in the pair. */
};

/**
 * @class ColliderPairHash
 * @brief Custom hash function for collider pairs.
 */
struct ColliderPairHash
{
    /**
     * @brief Calculate a hash value for a collider pair.
     * @param pair The collider pair to hash.
     * @return The hash value based on the individual hash values of the colliders.
     */
    std::size_t operator()(const ColliderPair &pair) const
    {
        std::size_t hashA = Collider{}(pair.ColA);
        std::size_t hashB = Collider{}(pair.ColB);

        // XOR to create a combined hash value
        return hashA ^ hashB;
    }
};

/**
 * @struct ColliderPairEqual
 * @brief Custom equality comparison for collider pairs.
 */
struct ColliderPairEqual
{
    /**
     * @brief Check if two collider pairs are equal.
     * @param a The first collider pair.
     * @param b The second collider pair.
     * @return `true` if the pairs are equal, `false` otherwise.
     */
    bool operator()(const ColliderPair &a, const ColliderPair &b) const
    {
        return ColliderPairHash{}(a) == ColliderPairHash{}(b);
    }
};

/**
 * @class ContactListener
 * @brief An abstract class for handling collision events.
 */
class ContactListener
{
public:
    /**
     * @brief Called when a collision begins.
     * @param col1 The first collider involved in the collision.
     * @param col2 The second collider involved in the collision.
     */
    virtual void BeginContact(Collider &col1, Collider &col2) noexcept = 0;

    /**
     * @brief Called when a collision ends.
     * @param col1 The first collider involved in the collision.
     * @param col2 The second collider involved in the collision.
     */
    virtual void EndContact(Collider &col1, Collider &col2) noexcept = 0;
};