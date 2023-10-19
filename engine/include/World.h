#pragma once

#include "Body.h"

#include <vector>

/**
 * @brief Represents a reference to a body in the world.
 * @note It consists of an index and a generation index.
 */
struct BodyRef {
    std::size_t Index;
    std::size_t GenIndex;

    /**
     * @brief Check if two BodyRef instances are equal.
     * @param other The BodyRef to compare with.
     * @return true if the BodyRefs are equal, false otherwise.
     */
    constexpr bool operator==(const BodyRef &other) const {
        return (Index == other.Index) && (GenIndex == other.GenIndex);
    }
};

/**
 * @brief Represents the physics world containing bodies and interactions.
 * @note This class manages the simulation of physics entities.
 */
class World {
private:
    std::vector<Body> _bodies;

public:
    std::vector<size_t> GenIndices;

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
};