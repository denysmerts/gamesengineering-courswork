#ifndef GAMEOVERSCREEN_H
#define GAMEOVERSCREEN_H

#include <SFML/Graphics.hpp>

using namespace sf;

class GameOverScreen {
public:
    GameOverScreen(const Font& font, const Vector2u& windowSize);
    void render(RenderWindow& window);

private:
    Text gameOverText;
};

#endif 
