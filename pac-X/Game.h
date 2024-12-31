#include "Ghost.h"

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
    sf::Clock clock;

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

    // ghost properties
    static const int NUM_GHOSTS = 4;
    std::array<Ghost, NUM_GHOSTS> ghosts;
    bool powerMode;
    float powerModeTimer;
    static constexpr float POWER_MODE_DURATION = 6.0f;

    // Add these new private methods:
    void initializeGhosts();
    void updateGhosts(float deltaTime);
    void checkGhostCollisions();
    const float GHOST_RELEASE_INTERVAL = 5.0f;
    float powerPelletBlinkTimer = 0.0f;
    bool powerPelletBlinkState = false;


public:
    Game(sf::RenderWindow& window);

    void handleEvent(const sf::Event& event);
    void handleInput(float deltaTime);
    void update(float deltaTime);
	void render();  
    int getScore() const { return score; }
    void reset();
    Direction currentDirection; // Add this line
    Direction queuedDirection;
};