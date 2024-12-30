#pragma once
#pragma once
#include <SFML/Graphics.hpp>

enum class GhostType {
    BLINKY,  // Red ghost - direct chase
    PINKY,   // Pink ghost - intercept
    INKY,    // Blue ghost - ambush
    CLYDE    // Orange ghost - random
};

enum class GhostState {
    CHASE,
    SCATTER,
    FRIGHTENED,
    EATEN
};

class Ghost {
public:
    Ghost(GhostType type, const sf::Vector2f& startPos, float cellSize);

    void update(float deltaTime, const sf::Vector2f& pacmanPos);
    void render(sf::RenderWindow& window);
    void setFrightened(bool frightened);
    bool isColliding(const sf::Vector2f& pacmanPos, float pacmanRadius);
    void reset();


    // Getters
    sf::Vector2f getPosition() const { return position; }
    GhostState getState() const { return currentState; }
    bool isFrightened() const { return currentState == GhostState::FRIGHTENED; }

private:
    void updateMovement(float deltaTime, const sf::Vector2f& pacmanPos);
    sf::Vector2f calculateTarget(const sf::Vector2f& pacmanPos);
    void updateState(float deltaTime);

    GhostType type;
    GhostState currentState;
    sf::CircleShape shape;
    sf::Vector2f position;
    sf::Vector2f startPosition;
    sf::Vector2f direction;

    float speed;
    float cellSize;
    float stateTimer;

    // State durations in seconds
    static constexpr float CHASE_DURATION = 20.0f;
    static constexpr float SCATTER_DURATION = 7.0f;
    static constexpr float FRIGHTENED_DURATION = 6.0f;

    static constexpr float NORMAL_SPEED = 100.0f;
    static constexpr float FRIGHTENED_SPEED = 75.0f;
    static constexpr float EATEN_SPEED = 150.0f;
};