#pragma once

#include "Shape.h"
#include "Refs.h"

#include <variant>

/**
 * @file Collider.h
 * @brief This file defines the `Shape`, `Collider`, `ColliderPair`, and `ContactListener` structures
 * and a `ColliderPairHash` class for managing physics-related data and collision events in a simulation.
 */



/**
 * @class Collider
 * @brief Represents a collider in a physics simulation.
 */
class Collider
{
public:
    std::variant<Math::CircleF, Math::RectangleF, Math::PolygonF> Shape{
            Math::CircleF(Math::Vec2F::Zero(), 1)}; /**< The shape associated with the collider. */
    //float bounciness = 1.f;
    //float friction = 1.f;
    bool IsSensor = true; /**< Flag indicating if the collider is a sensor (non-physical). */
    bool IsAttached = false; /**< Flag indicating if the collider is attached to a body. */
    BodyRef BodyRef; /**< Reference to the body associated with the collider. */
    
    Math::Vec2F BodyPosition = Math::Vec2F::Zero();
    
    [[nodiscard]] Math::RectangleF Collider::GetBounds() const noexcept
    {
        switch (Shape.index())
        {
            case static_cast<int>(Math::ShapeType::Circle):
            {
                auto circle = std::get<Math::CircleF>(Shape);
                return Math::RectangleF::FromCenter(circle.Center(), {circle.Radius(), circle.Radius()}) + BodyPosition;
            }
            case static_cast<int>(Math::ShapeType::Rectangle):
            {
                return std::get<Math::RectangleF>(Shape) + BodyPosition;
            }
//            case static_cast<int>(Math::ShapeType::Polygon):
//            {
//                float minX = std::numeric_limits<float>::max();
//                float minY = std::numeric_limits<float>::max();
//                float maxX = std::numeric_limits<float>::min();
//                float maxY = std::numeric_limits<float>::min();
//                Math::PolygonF polygon = GetPolygon();
//
//                for (auto &vertex: polygon.Vertices())
//                {
//                    minX = std::min(minX, vertex.X);
//                    minY = std::min(minY, vertex.Y);
//                    maxX = std::max(maxX, vertex.X);
//                    maxY = std::max(maxY, vertex.Y);
//                }
//
//                return Math::RectangleF{Math::Vec2F{minX, minY}, Math::Vec2F{maxX, maxY}} + _position;
//            }
        }
    }
};

/**
 * @struct ColliderPair
 * @brief Represents a pair of colliders involved in a collision.
 */
struct ColliderRefPair // todo; colliderRef au lieu de collider
{
    ColliderRef ColRefA; /**< The first colliderRef in the pair. */
    ColliderRef ColRefB; /**< The second colliderRef in the pair. */
    
    constexpr bool operator==(const ColliderRefPair &other) const
    {
        return ColRefA.Index == other.ColRefA.Index && ColRefB.Index == other.ColRefB.Index ||
               ColRefA.Index == other.ColRefB.Index && ColRefB.Index == other.ColRefA.Index;
    }
};

/**
 * @class ColliderPairHash
 * @brief Custom hash function for collider pairs.
 */
struct ColliderRefPairHash
{
    /**
     * @brief Calculate a hash value for a collider pair.
     * @param pair The collider pair to hash.
     * @return The hash value based on the individual hash values of the colliders.
     */
    constexpr std::size_t operator()(const ColliderRefPair &pair) const
    {
        const std::size_t hashA = std::hash<size_t>{}(pair.ColRefA.Index);
        const std::size_t hashB = std::hash<size_t>{}(pair.ColRefB.Index);
        
        // XOR to create a combined hash valueS
        return hashA ^ hashB;
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
    virtual void BeginContact(ColliderRef &colRef1, ColliderRef &colRef2) noexcept = 0;
    
    /**
     * @brief Called when a collision ends.
     * @param col1 The first collider involved in the collision.
     * @param col2 The second collider involved in the collision.
     */
    virtual void EndContact(ColliderRef &colRef1, ColliderRef &colRef2) noexcept = 0;
};