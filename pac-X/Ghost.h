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

    Ghost(const Ghost& other) :
        type(other.type),
        currentState(other.currentState),
        shape(other.shape),
        position(other.position),
        startPosition(other.startPosition),
        direction(other.direction),
        speed(other.speed),
        cellSize(other.cellSize),
        stateTimer(other.stateTimer),
        isReleased(other.isReleased),
        releaseTimer(other.releaseTimer),
        blinkState(other.blinkState),
        blinkTimer(other.blinkTimer) {
    }

    Ghost& operator=(const Ghost& other) {
        if (this == &other) {
            return *this;
        }
        type = other.type;
        currentState = other.currentState;
        shape = other.shape;
        position = other.position;
        startPosition = other.startPosition;
        direction = other.direction;
        speed = other.speed;
        cellSize = other.cellSize;
        stateTimer = other.stateTimer;
        isReleased = other.isReleased;
        releaseTimer = other.releaseTimer;
        blinkState = other.blinkState;
        blinkTimer = other.blinkTimer;
        return *this;
    }

    void update(float deltaTime, const sf::Vector2f& pacmanPos);
    void render(sf::RenderWindow& window);
    void setFrightened(bool frightened);
    bool isColliding(const sf::Vector2f& pacmanPos, float pacmanRadius);
    void reset();
    bool checkCollision(const sf::Vector2f& newPos, const std::array<std::array<int, 28>, 31>& maze, float offsetX, float offsetY, float cellSize);

    // Getters

    bool getBlinkState() const { return blinkState; }
    float getBlinkTimer() const { return blinkTimer; }
    void setBlinkState(bool state) { blinkState = state; }
    void setBlinkTimer(float timer) { blinkTimer = timer; }
   
    float getReleaseTimer() const { return releaseTimer; }
    void setReleaseTimer(float timer) { releaseTimer = timer; }
    bool getIsReleased() const { return isReleased; }
    void setIsReleased(bool released) { isReleased = released; }

    sf::Vector2f getPosition() const { return position; }
    GhostState getState() const { return currentState; }
    bool isFrightened() const { return currentState == GhostState::FRIGHTENED; }
    float getStartPositionY() const { return startPosition.y; }
    void setPositionY(float y) { position.y = y; }
    void setMazeData(const std::array<std::array<int, 28>, 31>& maze) { mazeData = maze; }
        //void setMazeOffsets(float offsetX, float offsetY);
    void setMazeOffsets(float offsetX, float offsetY) {
        mazeOffsetX = offsetX;
        mazeOffsetY = offsetY;
    }
    void setShapePosition(const sf::Vector2f& pos) { shape.setPosition(pos); }

private:
    float mazeOffsetX;
    float mazeOffsetY;
    void updateMovement(float deltaTime, const sf::Vector2f& pacmanPos);
    sf::Vector2f calculateTarget(const sf::Vector2f& pacmanPos);
    void updateState(float deltaTime);
    std::array<std::array<int, 28>, 31> mazeData;

    GhostType type;
    GhostState currentState;
    sf::CircleShape shape;
    sf::Vector2f position;
    sf::Vector2f startPosition;
    sf::Vector2f direction;
    float speed;
    float cellSize;
    float stateTimer;

    // Moved from public to private
    bool isReleased = false;
    float releaseTimer = 0.0f;
    bool blinkState = false;
    float blinkTimer = 0.0f;

    // Constants
    static constexpr float BLINK_INTERVAL = 0.2f;
    static constexpr float CHASE_DURATION = 20.0f;
    static constexpr float SCATTER_DURATION = 7.0f;
    static constexpr float FRIGHTENED_DURATION = 6.0f;
    static constexpr float NORMAL_SPEED = 100.0f;
    static constexpr float FRIGHTENED_SPEED = 75.0f;
    static constexpr float EATEN_SPEED = 150.0f;
};