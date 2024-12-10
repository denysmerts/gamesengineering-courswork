#ifndef WINSCREEN_H
#define WINSCREEN_H

#include <SFML/Graphics.hpp>

class WinScreen {
public:
    WinScreen(const sf::Font& font);
    void render(sf::RenderWindow& window);

private:
    sf::Text winText;
};

#endif // WINSCREEN_H