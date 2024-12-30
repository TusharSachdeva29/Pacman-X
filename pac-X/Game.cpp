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

Game::Game(sf::RenderWindow& gameWindow) : window(gameWindow), isPaused(false), score(0) {
    // Initialize Pac-Man
    pacman.setRadius(CELL_SIZE / 2.0f - 2.0f); // Slightly smaller than cell
    pacman.setFillColor(sf::Color::Yellow);
    pacman.setOrigin(sf::Vector2f(CELL_SIZE / 2.0f - 2.0f, CELL_SIZE / 2.0f - 2.0f));

    position = sf::Vector2f(CELL_SIZE * 14.0f, CELL_SIZE * 23.0f);
    pacman.setPosition(position);

    pacmanSpeed = 150.0f;
    pacmanAngle = 0.0f;

    // Initialize game elements
    initializeMaze();
    createWalls();
    createDots();
}
void Game::createDots() {
    float dotRadius = 2.0f;
    float powerPelletRadius = 8.0f;

    for (int y = 0; y < MAZE_HEIGHT; ++y) {
        for (int x = 0; x < MAZE_WIDTH; ++x) {
            if (mazeData[y][x] == 0) {  // Empty space
                // Create regular dot
                sf::CircleShape dot(dotRadius);
                dot.setFillColor(sf::Color::White);
                dot.setPosition(sf::Vector2f(
                    x * CELL_SIZE + CELL_SIZE / 2 - dotRadius,
                    y * CELL_SIZE + CELL_SIZE / 2 - dotRadius
                ));

                // Special positions for power pellets
                if ((x == 1 && y == 3) || (x == 26 && y == 3) ||
                    (x == 1 && y == 23) || (x == 26 && y == 23)) {
                    dot.setRadius(powerPelletRadius);
                    dot.setPosition(sf::Vector2f(
                        x * CELL_SIZE + CELL_SIZE / 2 - powerPelletRadius,
                        y * CELL_SIZE + CELL_SIZE / 2 - powerPelletRadius
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
    return sf::Vector2f(
        static_cast<float>(static_cast<int>(position.x / CELL_SIZE)),
        static_cast<float>(static_cast<int>(position.y / CELL_SIZE))
    );
}


void Game::createWalls() {
    for (int i = 0; i < MAZE_HEIGHT; ++i) {
        for (int j = 0; j < MAZE_WIDTH; ++j) {
            if (mazeData[i][j] == 1) {
                sf::RectangleShape wall(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                wall.setFillColor(sf::Color::Blue);
                wall.setPosition(sf::Vector2f(j * CELL_SIZE, i * CELL_SIZE)); // Fix for E0415 and E0140
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

    sf::Vector2f movement(0.0f, 0.0f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        movement.x = -1.0f;
        pacmanAngle = 180.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        movement.x = 1.0f;
        pacmanAngle = 0.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        movement.y = -1.0f;
        pacmanAngle = 270.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        movement.y = 1.0f;
        pacmanAngle = 90.0f;
    }

    if (movement.x != 0.0f && movement.y != 0.0f) {
        movement /= std::sqrt(2.0f);
    }

    // Test new position before moving
    sf::Vector2f newPosition = position + movement * pacmanSpeed * deltaTime;
    if (!checkCollision(newPosition)) {
        position = newPosition;
        pacman.setPosition(position);
        pacman.setRotation(sf::degrees(pacmanAngle));
    }
}

void Game::update(float deltaTime) {
    if (isPaused) return;
    // We'll add collision detection and other game logic here later
	checkDotCollection();
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