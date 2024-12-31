#include "Ghost.h"
#include <cmath>

Ghost::Ghost(GhostType type, const sf::Vector2f& startPos, float cellSize)
    : type(type),
    currentState(GhostState::SCATTER),
    position(startPos),
    startPosition(startPos),
    cellSize(cellSize),
    speed(NORMAL_SPEED),
    stateTimer(SCATTER_DURATION),
    direction(0.0f, 0.0f)
{
    shape.setRadius(cellSize / 2.4f);
    shape.setOrigin(sf::Vector2f(shape.getRadius(), shape.getRadius()));

    // Set color based on ghost type
    switch (type) {
    case GhostType::BLINKY:
        shape.setFillColor(sf::Color::Red);
        break;
    case GhostType::PINKY:
        shape.setFillColor(sf::Color::Magenta);
        break;
    case GhostType::INKY:
        shape.setFillColor(sf::Color::Cyan);
        break;
    case GhostType::CLYDE:
        shape.setFillColor(sf::Color(255, 165, 0)); // Orange
        break;
    }
}



void Ghost::update(float deltaTime, const sf::Vector2f& pacmanPos) {
    if (currentState == GhostState::FRIGHTENED) {
        blinkTimer += deltaTime;
        if (blinkTimer >= BLINK_INTERVAL) {
            blinkTimer = 0.0f;
            blinkState = !blinkState;
            shape.setFillColor(blinkState ? sf::Color::Green : sf::Color::Magenta);
        }
    }
    updateState(deltaTime);
    updateMovement(deltaTime, pacmanPos);
    shape.setPosition(position);
}

void Ghost::updateState(float deltaTime) {
    stateTimer -= deltaTime;

    if (stateTimer <= 0.0f) {
        switch (currentState) {
        case GhostState::CHASE:
            currentState = GhostState::SCATTER;
            stateTimer = SCATTER_DURATION;
            break;
        case GhostState::SCATTER:
            currentState = GhostState::CHASE;
            stateTimer = CHASE_DURATION;
            break;
        case GhostState::FRIGHTENED:
            currentState = GhostState::CHASE;
            stateTimer = CHASE_DURATION;
            speed = NORMAL_SPEED;
            shape.setFillColor(sf::Color::White);
            break;
        }
    }
}

void Ghost::updateMovement(float deltaTime, const sf::Vector2f& pacmanPos) {
    sf::Vector2f target = calculateTarget(pacmanPos);

    // Calculate direction to target
    sf::Vector2f toTarget = target - position;
    float length = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);

    if (length > 0) {
        direction = sf::Vector2f(toTarget.x / length, toTarget.y / length);
    }

    // Move ghost
    position += direction * speed * deltaTime;
}

sf::Vector2f Ghost::calculateTarget(const sf::Vector2f& pacmanPos) {
    switch (currentState) {
    case GhostState::FRIGHTENED:
        // Random movement
        return sf::Vector2f(
            static_cast<float>(rand() % 28) * cellSize,
            static_cast<float>(rand() % 31) * cellSize
        );

    case GhostState::SCATTER:
        // Return to corner based on ghost type
        switch (type) {
        case GhostType::BLINKY: return sf::Vector2f(26 * cellSize, 0);
        case GhostType::PINKY:  return sf::Vector2f(2 * cellSize, 0);
        case GhostType::INKY:   return sf::Vector2f(26 * cellSize, 30 * cellSize);
        case GhostType::CLYDE:  return sf::Vector2f(0, 30 * cellSize);
        }

    case GhostState::CHASE:
        // Each ghost has unique targeting behavior
        switch (type) {
        case GhostType::BLINKY:
            // Direct chase
            return pacmanPos;

        case GhostType::PINKY:
            // Target 4 tiles ahead of Pac-Man
            return pacmanPos + direction * (4 * cellSize);

        case GhostType::INKY:
            // Complex targeting (simplified for now)
            return pacmanPos;

        case GhostType::CLYDE:
            // Random movement when far, scatter when close
            float distance = std::sqrt(
                std::pow(position.x - pacmanPos.x, 2) +
                std::pow(position.y - pacmanPos.y, 2)
            );
            return (distance > 8 * cellSize) ? pacmanPos : startPosition;
        }

    case GhostState::EATEN:
        return startPosition;
    }

    return position; // Fallback
}

void Ghost::setFrightened(bool frightened) {
    if (frightened && currentState != GhostState::FRIGHTENED) {
        currentState = GhostState::FRIGHTENED;
        stateTimer = FRIGHTENED_DURATION;
        speed = FRIGHTENED_SPEED;
        shape.setFillColor(sf::Color::Green); // Different color
    }
}

bool Ghost::checkCollision(const sf::Vector2f& newPos, const std::array<std::array<int, 28>, 31>& maze, float offsetX, float offsetY, float cellSize) {
    float radius = shape.getRadius();
    std::vector<sf::Vector2f> corners = {
        sf::Vector2f(newPos.x - radius, newPos.y - radius),
        sf::Vector2f(newPos.x + radius, newPos.y - radius),
        sf::Vector2f(newPos.x - radius, newPos.y + radius),
        sf::Vector2f(newPos.x + radius, newPos.y + radius)
    };

    for (const auto& corner : corners) {
        int gridX = static_cast<int>((corner.x - offsetX) / cellSize);
        int gridY = static_cast<int>((corner.y - offsetY) / cellSize);

        if (gridX >= 0 && gridX < 28 && gridY >= 0 && gridY < 31) {
            if (maze[gridY][gridX] == 1) return true;
        }
    }
    return false;
}

bool Ghost::isColliding(const sf::Vector2f& pacmanPos, float pacmanRadius) {
    float distance = std::sqrt(
        std::pow(position.x - pacmanPos.x, 2) +
        std::pow(position.y - pacmanPos.y, 2)
    );
    return distance < (shape.getRadius() + pacmanRadius);
}

void Ghost::render(sf::RenderWindow& window) {
    window.draw(shape);
}

void Ghost::reset() {
    position = startPosition;
    currentState = GhostState::SCATTER;
    stateTimer = SCATTER_DURATION;
    speed = NORMAL_SPEED;
    direction = sf::Vector2f(0.0f, 0.0f);
}