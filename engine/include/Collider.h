#pragma once

#include "Shape.h"

#include "Refs.h"

struct Shape
{
    Math::ShapeType Type;
    union //todo: check std::variant
    {
        Math::CircleF *Circle;
        Math::RectangleF *Rectangle;
        Math::PolygonF *Polygon;
    };
};

class Collider
{
public:
    Shape *ColShape = nullptr;
    //float bounciness = 1.f;
    //float friction = 1.f;
    bool IsSensor = true;
    bool IsAttached = false;
    BodyRef BodyRef;
    int ID;
private:
    //todo: if no body ref then collider is not attached
    std::size_t _triggerNbr = 0;


public:

    [[nodiscard]] bool DoesTrigger() const noexcept
    { return _triggerNbr > 0; }

    void OnTriggerEnter() noexcept
    {
        _triggerNbr++;
    }

    std::size_t operator()(const Collider &collider) const
    {
        // Implement a custom hash function for Collider here.
        // You can combine the hash values of its members.
        // For example, if Collider has an int member called id:
        return std::hash<int>{}(collider.ID);
    }
};

//struct ColliderPair
//{
//    Collider ColA;
//    Collider ColB;
//
//    bool operator==(const ColliderPair &other) const
//    {
//        return (ColA == other.ColA && ColB == other.ColB) || (ColA == other.ColB && ColB == other.ColA);
//    }
//};
//
//// Implement a hash function for ColliderPair
//struct ColliderPairHash
//{
//    std::size_t operator()(const ColliderPair &pair) const
//    {
//        // You should combine the hash values of ColA and ColB.
//        std::size_t hashA = std::hash<Collider>{}(pair.ColA);
//        std::size_t hashB = std::hash<Collider>{}(pair.ColB);
//
//        // XOR the two hash values to combine them.
//        return hashA ^ hashB;
//    }
//};