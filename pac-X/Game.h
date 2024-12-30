#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <array>

class Game {
private:
    enum class Direction {
        NONE,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };
    sf::RenderWindow& window;

    // Pac-Man properties
    sf::CircleShape pacman;
    sf::Vector2f position;
    float pacmanSpeed;
    float pacmanAngle;

    // Maze properties
    static constexpr int MAZE_WIDTH = 28;
    static constexpr int MAZE_HEIGHT = 31;
    static constexpr float CELL_SIZE = 34.0f;

    // Game elements
    std::vector<sf::RectangleShape> walls;
    std::vector<sf::CircleShape> dots;
    std::vector<sf::CircleShape> powerPellets;
    std::array<std::array<int, MAZE_WIDTH>, MAZE_HEIGHT> mazeData;

    // Game state
    bool isPaused;
    int score;

    // Helper functions
    void initializeMaze();
    void createWalls();
    void createDots();
    bool checkCollision(const sf::Vector2f& newPos);
    void checkDotCollection();
    sf::Vector2f getGridPosition(const sf::Vector2f& position) const;
    bool canMove(Direction dir);
    void moveInDirection(Direction dir, float deltaTime);

public:
    Game(sf::RenderWindow& window);

    void handleEvent(const sf::Event& event);
    void handleInput(float deltaTime);
    void update(float deltaTime);
    void render();
    int getScore() const { return score; }
    Direction currentDirection; // Add this line
    Direction queuedDirection;
};