#pragma once

#include "Body.h"
#include "Collider.h"
#include <vector>
#include <unordered_set>

struct Association{ //todo:  demander prof
    BodyRef bRef;
    ColliderRef cRef;

    //RigidBody aRef;
    //OtherRef;

    //GetAssociationByRef();
};

/**
 * @brief Represents the physics world containing bodies and interactions.
 * @note This class manages the simulation of physics entities.
 */

class World {
private:
    std::vector<Body> _bodies; /**< A collection of all the bodies in the world. */
    std::vector<Collider> _colliders; /**< A collection of all the colliders in the world. */

    int _colliderIdCount = 0; /**< Counter for generating unique collider IDs. */

    std::unordered_set<ColliderRefPair, ColliderRefPairHash> _colRefPairs; /**< A set of colliderRef pairs for collision detection. */

    ContactListener* _contactListener = nullptr; /**< A listener for contact events between colliders. */

public:
    std::vector<size_t> BodyGenIndices; /**< Indices of generated bodies. */
    std::vector<size_t> ColliderGenIndices; /**< Indices of generated colliders. */

    /**
     * @brief Default constructor for the _world class.
     */
    World() noexcept = default;

    /**
     * @brief Set up the initial state of the world.
     */
    void SetUp() noexcept;

    /**
     * @brief Tear down the world and release resources.
     */
    void TearDown() noexcept;

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

    /**
     * @brief Create a new collider attached to a specific body in the world.
     * @param bodyRef The reference to the body that the collider will be attached to.
     * @return A reference to the created collider.
     */
    [[nodiscard]] ColliderRef CreateCollider(BodyRef bodyRef) noexcept;

    /**
     * @brief Get a reference to a collider in the world.
     * @param colRef The reference to the desired collider.
     * @return A reference to the specified collider.
     */
    [[nodiscard]] Collider &GetCollider(ColliderRef colRef);

    /**
     * @brief Destroy a collider in the world.
     * @param colRef The reference to the collider to be destroyed.
     */
    void DestroyCollider(ColliderRef colRef);

    /**
     * @brief Check if two colliders overlap.
     * @param colA The first collider.
     * @param colB The second collider.
     * @return true if the colliders overlap, false otherwise.
     */
    [[nodiscard]] bool Overlap(const Collider& colA, const Collider& colB);

    /**
     * @brief Set a contact listener to receive collision events.
     * @param listener A pointer to the contact listener object.
     */
    void SetContactListener(ContactListener* listener) {
        _contactListener = listener;
    }
};