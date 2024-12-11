#ifndef WINSCREEN_H
#define WINSCREEN_H

#include <SFML/Graphics.hpp>

using namespace sf;

class WinScreen {
public:
    WinScreen(const Font& font, const Vector2u& windowSize); // Use window size
    void render(RenderWindow& window);

private:
    Text winText;
};

#endif 
