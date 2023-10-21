#pragma once

#include "Body.h"
#include "Collider.h"
#include <vector>
#include <unordered_set>

/**
 * @brief Represents the physics world containing bodies and interactions.
 * @note This class manages the simulation of physics entities.
 */
class World {
private:
    std::vector<Body> _bodies;
    std::vector<Collider> _colliders;

    int _colliderIdCount = 0;

    std::unordered_set<ColliderPair, ColliderPairHash, ColliderPairEqual> _colPairs;

public:
    std::vector<size_t> BodyGenIndices;
    std::vector<size_t> ColliderGenIndices;

    /**
     * @brief Default constructor for the World class.
     */
    World() noexcept = default;

    /**
     * @brief Set up the initial state of the world.
     */
    void SetUp() noexcept;

    /**
     * @brief Update the simulation state of the world over a time step.
     * @param deltaTime The time step for the simulation.
     */
    void Update(float deltaTime) noexcept;

    /**
     * @brief Create a new body in the world.
     * @return A reference to the created body.
     */
    [[nodiscard]] BodyRef CreateBody() noexcept;

    /**
     * @brief Destroy a body in the world.
     * @param bodyRef The reference to the body to be destroyed.
     */
    void DestroyBody(BodyRef bodyRef);

    /**
     * @brief Get a reference to a body in the world.
     * @param bodyRef The reference to the desired body.
     * @return A reference to the specified body.
     */
    [[nodiscard]] Body &GetBody(BodyRef bodyRef);

    [[nodiscard]] ColliderRef CreateCollider(BodyRef bodyRef) noexcept;

    [[nodiscard]] Collider &GetCollider(ColliderRef colRef);

    void DestroyCollider(ColliderRef colRef);

    bool Overlap(const Collider& colA, const Collider& colB) const;

};