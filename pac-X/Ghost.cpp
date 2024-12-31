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
            // Restore original color based on ghost type
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
            break;
        }
    }
}
void Ghost::updateMovement(float deltaTime, const sf::Vector2f& pacmanPos) {
    if (!isReleased) return;

    // Get current grid position
    sf::Vector2f currentGridPos = {
        std::floor((position.x - mazeOffsetX) / cellSize) * cellSize + mazeOffsetX + cellSize / 2,
        std::floor((position.y - mazeOffsetY) / cellSize) * cellSize + mazeOffsetY + cellSize / 2
    };

    // Check if we're close to grid center
    float distToGridCenter = std::sqrt(
        std::pow(position.x - currentGridPos.x, 2) +
        std::pow(position.y - currentGridPos.y, 2)
    );

    bool atIntersection = distToGridCenter < 2.0f;

    if (atIntersection || direction == sf::Vector2f(0, 0)) {
        sf::Vector2f target = calculateTarget(pacmanPos);

        // Possible directions
        std::vector<sf::Vector2f> possibleDirs = {
            {1, 0}, {-1, 0}, {0, 1}, {0, -1}
        };

        // Remove opposite direction unless no other choice
        if (direction != sf::Vector2f(0, 0)) {
            possibleDirs.erase(
                std::remove_if(possibleDirs.begin(), possibleDirs.end(),
                    [this](const sf::Vector2f& dir) {
                        return dir.x == -direction.x && dir.y == -direction.y;
                    }),
                possibleDirs.end()
            );
        }

        // Filter out directions that would hit walls
        possibleDirs.erase(
            std::remove_if(possibleDirs.begin(), possibleDirs.end(),
                [this, &currentGridPos](const sf::Vector2f& dir) {
                    sf::Vector2f testPos = currentGridPos + sf::Vector2f(dir.x * cellSize, dir.y * cellSize);
                    return checkCollision(testPos, mazeData, mazeOffsetX, mazeOffsetY, cellSize);
                }),
            possibleDirs.end()
        );

        if (!possibleDirs.empty()) {
            // Choose direction closest to target
            float bestDistance = std::numeric_limits<float>::max();
            sf::Vector2f bestDir;

            for (const auto& dir : possibleDirs) {
                sf::Vector2f nextPos = currentGridPos + sf::Vector2f(dir.x * cellSize, dir.y * cellSize);
                float dist = std::sqrt(
                    std::pow(nextPos.x - target.x, 2) +
                    std::pow(nextPos.y - target.y, 2)
                );

                if (dist < bestDistance) {
                    bestDistance = dist;
                    bestDir = dir;
                }
            }
            direction = bestDir;
        }
    }

    // Move in current direction
    sf::Vector2f newPos = position + direction * speed * deltaTime;
    if (!checkCollision(newPos, mazeData, mazeOffsetX, mazeOffsetY, cellSize)) {
        position = newPos;
    }
    else {
        // If we hit a wall, stop and wait for next intersection
        direction = sf::Vector2f(0, 0);
    }

    shape.setPosition(position);
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
    isReleased = false;

    // Reset to original color
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