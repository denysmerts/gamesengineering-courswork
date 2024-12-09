#ifndef SETTINGSSCREEN_H
#define SETTINGSSCREEN_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>

using namespace std;
using namespace sf;

class SettingsScreen {
private:
    Text fullscreenText;
    Text fullscreenLeftArrow;
    Text fullscreenRightArrow;
    Text backText;
    Font& font;
    bool& isFullscreen;

public:
    SettingsScreen(Font& font, bool& fullscreenFlag);

    void handleEvent(Event& event, RenderWindow& window, bool& backToStartScreen);
    void render(RenderWindow& window);

private:
    Text createText(const string& content, Font& font, unsigned int size, Color color);
    void centerText(Text& text, const VideoMode& mode);
    void applyResolution(RenderWindow& window, VideoMode& videoMode, bool isFullscreen);
};

#endif 
