#ifndef GAMEOVERSCREEN_H
#define GAMEOVERSCREEN_H

#include <SFML/Graphics.hpp>

class GameOverScreen {
public:
    GameOverScreen(const sf::Font& font);
    void render(sf::RenderWindow& window);

private:
    sf::Text gameOverText;
};

#endif // GAMEOVERSCREEN_H
