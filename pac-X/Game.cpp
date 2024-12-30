#include "Game.h"
#include <iostream>

// The maze layout (0 = empty, 1 = wall)
const std::array<std::array<int, 28>, 31> MAZE_TEMPLATE = { {
    {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}},
    {{1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1}},
    {{1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1}},
    {{1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1}},
    {{1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1}},
    {{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}},
    {{1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1}},
    {{1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1}},
    {{1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1}},
    {{1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1}},
    {{1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1}},
    {{1,1,1,1,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,1,1,1,1}},
    {{1,1,1,1,1,1,0,1,1,0,1,1,1,0,0,1,1,1,0,1,1,0,1,1,1,1,1,1}},
    {{1,1,1,1,1,1,0,1,1,0,1,0,0,0,0,0,0,1,0,1,1,0,1,1,1,1,1,1}},
    {{0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0}},
    {{1,1,1,1,1,1,0,1,1,0,1,0,0,0,0,0,0,1,0,1,1,0,1,1,1,1,1,1}},
    {{1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1}},
    {{1,1,1,1,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,1,1,1,1}},
    {{1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1}},
    {{1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1}},
    {{1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1}},
    {{1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1}},
    {{1,1,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,1}},
    {{1,1,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,1}},
    {{1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1}},
    {{1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1}},
    {{1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1}},
    {{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}},
    {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}}
} };

Game::Game(sf::RenderWindow& gameWindow) :
    window(gameWindow),
    isPaused(false),
    score(0),
    currentDirection(Direction::NONE),
    queuedDirection(Direction::NONE) {

    float mazeWidth = MAZE_WIDTH * CELL_SIZE;
    float mazeHeight = MAZE_HEIGHT * CELL_SIZE;
    sf::Vector2u windowSize = window.getSize();
    float offsetX = (windowSize.x - mazeWidth) / 2.0f;
    float offsetY = (windowSize.y - mazeHeight) / 2.0f;

    // Initialize Pac-Man with larger size
    pacman.setRadius(CELL_SIZE / 2.4f);
    pacman.setFillColor(sf::Color::Yellow);
    pacman.setOrigin(sf::Vector2f(pacman.getRadius(), pacman.getRadius()));

    position = sf::Vector2f(
        CELL_SIZE * 14.0f + offsetX,
        CELL_SIZE * 15.0f + offsetY
    );
    pacman.setPosition(position);

    pacmanSpeed = 120.0f;
    pacmanAngle = 0.0f;

    initializeMaze();
    createWalls();
    createDots();
}

void Game::createDots() {
    sf::Vector2u windowSize = window.getSize();
    float mazeWidth = MAZE_WIDTH * CELL_SIZE;
    float mazeHeight = MAZE_HEIGHT * CELL_SIZE;

    // Calculate the offset needed to center the maze
    float offsetX = (windowSize.x - mazeWidth) / 2.0f;
    float offsetY = (windowSize.y - mazeHeight) / 2.0f;

    float dotRadius = 2.0f;
    float powerPelletRadius = 9.0f;

    for (int y = 0; y < MAZE_HEIGHT; ++y) {
        for (int x = 0; x < MAZE_WIDTH; ++x) {
            if (y == 14) continue;
            if (mazeData[y][x] == 0) {  // Empty space
                // Create regular dot
                sf::CircleShape dot(dotRadius);
                dot.setFillColor(sf::Color::White);
                dot.setPosition(sf::Vector2f(
                    x * CELL_SIZE + offsetX + CELL_SIZE / 2 - dotRadius,
                    y * CELL_SIZE + offsetY + CELL_SIZE / 2 - dotRadius
                ));

                // Special positions for power pellets
                if ((x == 1 && y == 3) || (x == 26 && y == 3) ||
                    (x == 1 && y == 26) || (x == 26 && y == 26)) {
                    dot.setRadius(powerPelletRadius);
                    dot.setPosition(sf::Vector2f(
                        x * CELL_SIZE + offsetX + CELL_SIZE / 2 - powerPelletRadius,
                        y * CELL_SIZE + offsetY + CELL_SIZE / 2 - powerPelletRadius
                    ));
                    powerPellets.push_back(dot);
                }
                else {
                    dots.push_back(dot);
                }
            }
        }
    }
}




bool Game::checkCollision(const sf::Vector2f& newPos) {
    // Get the grid positions for each corner of Pac-Man
    float radius = pacman.getRadius();
    std::vector<sf::Vector2f> corners = {
        sf::Vector2f(newPos.x - radius, newPos.y - radius),
        sf::Vector2f(newPos.x + radius, newPos.y - radius),
        sf::Vector2f(newPos.x - radius, newPos.y + radius),
        sf::Vector2f(newPos.x + radius, newPos.y + radius)
    };

    for (const auto& corner : corners) {
        sf::Vector2f gridPos = getGridPosition(corner);
        int gridX = static_cast<int>(gridPos.x);
        int gridY = static_cast<int>(gridPos.y);

        if (gridX >= 0 && gridX < MAZE_WIDTH && gridY >= 0 && gridY < MAZE_HEIGHT) {
            if (mazeData[gridY][gridX] == 1) {
                return true; // Collision detected
            }
        }
    }

    return false;
}


void Game::checkDotCollection() {
	sf::Vector2f gridPos = getGridPosition(position);
	int gridX = static_cast<int>(gridPos.x);
	int gridY = static_cast<int>(gridPos.y);
	for (auto it = dots.begin(); it != dots.end(); ) {
		sf::Vector2f dotPos = it->getPosition();
		sf::Vector2f dotGridPos = getGridPosition(dotPos);
		int dotGridX = static_cast<int>(dotGridPos.x);
		int dotGridY = static_cast<int>(dotGridPos.y);
		if (gridX == dotGridX && gridY == dotGridY) {
			it = dots.erase(it);
			score += 10;
		}
		else {
			++it;
		}
	}
	for (auto it = powerPellets.begin(); it != powerPellets.end(); ) {
		sf::Vector2f pelletPos = it->getPosition();
		sf::Vector2f pelletGridPos = getGridPosition(pelletPos);
		int pelletGridX = static_cast<int>(pelletGridPos.x);
		int pelletGridY = static_cast<int>(pelletGridPos.y);
		if (gridX == pelletGridX && gridY == pelletGridY) {
			it = powerPellets.erase(it);
			score += 50;
		}
		else {
			++it;
		}
	}
}


sf::Vector2f Game::getGridPosition(const sf::Vector2f& position) const {
    // Calculate maze offsets
    float mazeWidth = MAZE_WIDTH * CELL_SIZE;
    float mazeHeight = MAZE_HEIGHT * CELL_SIZE;
    sf::Vector2u windowSize = window.getSize();
    float offsetX = (windowSize.x - mazeWidth) / 2.0f;
    float offsetY = (windowSize.y - mazeHeight) / 2.0f;

    // Adjust position by the offset before converting to grid coordinates
    return sf::Vector2f(
        static_cast<float>(static_cast<int>((position.x - offsetX) / CELL_SIZE)),
        static_cast<float>(static_cast<int>((position.y - offsetY) / CELL_SIZE))
    );
}

void Game::createWalls() {
    sf::Vector2u windowSize = window.getSize();
    float mazeWidth = MAZE_WIDTH * CELL_SIZE;
    float mazeHeight = MAZE_HEIGHT * CELL_SIZE;

    // Calculate the offset needed to center the maze
    float offsetX = (windowSize.x - mazeWidth) / 2.0f;
    float offsetY = (windowSize.y - mazeHeight) / 2.0f;

    for (int i = 0; i < MAZE_HEIGHT; ++i) {
        for (int j = 0; j < MAZE_WIDTH; ++j) {
            if (mazeData[i][j] == 1) {
                sf::RectangleShape wall(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                wall.setFillColor(sf::Color::Blue);
                wall.setPosition(sf::Vector2f(j * CELL_SIZE + offsetX, i * CELL_SIZE + offsetY));
                walls.push_back(wall);
            }
        }
    }
}

void Game::initializeMaze() {
    mazeData = MAZE_TEMPLATE;
}

void Game::handleEvent(const sf::Event& event) {
    if (event.is<sf::Event::KeyPressed>()) {
        if (event.getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::P) {
            isPaused = !isPaused;
        }
    }
}

void Game::handleInput(float deltaTime) {
    if (isPaused) return;

    // Only update queued direction when a key is newly pressed
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        queuedDirection = Direction::LEFT;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        queuedDirection = Direction::RIGHT;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        queuedDirection = Direction::UP;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        queuedDirection = Direction::DOWN;
    }

    // If no current direction, try to move in queued direction immediately
    if (currentDirection == Direction::NONE && queuedDirection != Direction::NONE) {
        if (canMove(queuedDirection)) {
            currentDirection = queuedDirection;
            queuedDirection = Direction::NONE;
        }
    }
}

void Game::update(float deltaTime) {
    if (isPaused) return;

    // Always try to move in queued direction if we have one
    if (queuedDirection != Direction::NONE) {
        if (canMove(queuedDirection)) {
            currentDirection = queuedDirection;
            queuedDirection = Direction::NONE;
        }
    }

    // Continue moving in current direction
    if (currentDirection != Direction::NONE) {
        moveInDirection(currentDirection, deltaTime);
    }

    checkDotCollection();
}


bool Game::canMove(Direction dir) {
    sf::Vector2f testPos = position;
    float moveAmount = CELL_SIZE / 4.0f;  // Small test movement

    switch (dir) {
    case Direction::LEFT:
        testPos.x -= moveAmount;
        break;
    case Direction::RIGHT:
        testPos.x += moveAmount;
        break;
    case Direction::UP:
        testPos.y -= moveAmount;
        break;
    case Direction::DOWN:
        testPos.y += moveAmount;
        break;
    default:
        return false;
    }

    return !checkCollision(testPos);
}


void Game::moveInDirection(Direction dir, float deltaTime) {
    sf::Vector2f movement(0.0f, 0.0f);
    float moveSpeed = pacmanSpeed * deltaTime;

    switch (dir) {
    case Direction::LEFT:
        movement.x = -moveSpeed;
        pacmanAngle = 180.0f;
        break;
    case Direction::RIGHT:
        movement.x = moveSpeed;
        pacmanAngle = 0.0f;
        break;
    case Direction::UP:
        movement.y = -moveSpeed;
        pacmanAngle = 270.0f;
        break;
    case Direction::DOWN:
        movement.y = moveSpeed;
        pacmanAngle = 90.0f;
        break;
    }

    sf::Vector2f newPos = position + movement;
    if (!checkCollision(newPos)) {
        position = newPos;
        pacman.setPosition(position);
        pacman.setRotation(sf::degrees(pacmanAngle));
    }
    else {
        // Hit a wall, stop moving in this direction
        currentDirection = Direction::NONE;
    }
}


void Game::render() {
    // Draw walls
    for (const auto& wall : walls) {
        window.draw(wall);
    }

    // Draw dots
    for (const auto& dot : dots) {
        window.draw(dot);
    }

    // Draw power pellets
    for (const auto& pellet : powerPellets) {
        window.draw(pellet);
    }

    // Draw Pac-Man
    window.draw(pacman);
}