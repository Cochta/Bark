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

    void OnTriggerExit() noexcept
    {
        _triggerNbr--;
    }

    std::size_t operator()(const Collider &collider) const
    {
        return std::hash<int>{}(collider.ID);
    }
};

struct ColliderPair
{
    Collider ColA;
    Collider ColB;
};

struct ColliderPairHash
{
    std::size_t operator()(const ColliderPair &pair) const
    {
        std::size_t hashA = Collider{}(pair.ColA);

        std::size_t hashB = Collider{}(pair.ColB);

        //XOR SAMER
        return hashA ^ hashB;
    }
};

struct ColliderPairEqual
{
    bool operator()(const ColliderPair &a, const ColliderPair &b) const
    {
        // Implement your own equality comparison logic for ColliderPair
        return ColliderPairHash{}(a) == ColliderPairHash{}(b);
    }
};