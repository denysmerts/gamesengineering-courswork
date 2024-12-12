#ifndef SETTINGSSCREEN_H
#define SETTINGSSCREEN_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <vector>

using namespace std;
using namespace sf;

class SettingsScreen {
private:
    Text fullscreenText, resolutionText, backText;
    Font& font;
    bool& isFullscreen;
    VideoMode& videoMode;
    vector<VideoMode> resolutions = {
        VideoMode(800, 600),
        VideoMode(1024, 768),
        VideoMode(1280, 720),
        VideoMode(1920, 1080)
    };
    int currentResolutionIndex = 3;
    RenderWindow& window; // Added this line

    void updateResolutionText();

public:
    SettingsScreen(Font& font, bool& fullscreenFlag, VideoMode& videoMode, RenderWindow& window);

    void handleEvent(Event& event, RenderWindow& window, bool& backToStartScreen);
    void render(RenderWindow& window);
    SettingsScreen(Font& font, bool& fullscreenFlag);

private:
    Text createText(const string& content, Font& font, unsigned int size, Color color);
    void centerText(Text& text, const VideoMode& mode);
    sf::Text fullscreenLeftArrow;
    sf::Text fullscreenRightArrow;
    void applyResolution(RenderWindow& window, VideoMode& videoMode, bool isFullscreen);
};

#endif