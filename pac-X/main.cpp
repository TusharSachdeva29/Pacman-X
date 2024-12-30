#include <SFML/Graphics.hpp>
#include<iostream>
#include "Game.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({1200,1000}), "Pac-Man");
    window.setFramerateLimit(60);

    Game game(window);

    // Clock for measuring frame time
    sf::Clock clock;

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            game.handleEvent(*event);
        }

        float deltaTime = clock.restart().asSeconds();

        game.handleInput(deltaTime);
        game.update(deltaTime);

        window.clear(sf::Color::Black);
        game.render();
        window.display();
    }

    return 0;
}