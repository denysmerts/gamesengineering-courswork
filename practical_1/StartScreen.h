#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <SFML/Graphics.hpp>

using namespace sf;

class StartScreen {
private:
    Sprite background;
    Text pressKeyText;
    Text settingsButton;
    Text quitGameButton;
    Font& font;

public:
    StartScreen(Font& font, Texture& backgroundTexture);

    void handleEvent(Event& event, RenderWindow& window, bool& startGame, bool& openSettings, bool& quitGame);
    void render(RenderWindow& window);
};

#endif 
