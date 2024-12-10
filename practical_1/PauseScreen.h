#ifndef PAUSESCREEN_H
#define PAUSESCREEN_H

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class PauseScreen {
private:
    Text pauseTitle;
    Text resumeText;
    Text quitText;
    RectangleShape overlay;
    Font& font;

public:
    PauseScreen(Font& font, RenderWindow& window);


    void handleEvent(Event& event, bool& resumeGame, bool& quitToMainMenu, bool& restartGame);

    void render(RenderWindow& window);
};

#endif 