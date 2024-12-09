#include "SettingsScreen.h"

using namespace std;
using namespace sf;

SettingsScreen::SettingsScreen(Font& font, bool& fullscreenFlag)
    : font(font), isFullscreen(fullscreenFlag) {
    // Fullscreen Text
    fullscreenText = createText(isFullscreen ? "Fullscreen: On" : "Fullscreen: Windowed", font, 50, Color::White);
    centerText(fullscreenText, VideoMode::getDesktopMode());
    fullscreenText.setPosition(fullscreenText.getPosition().x, fullscreenText.getPosition().y - 50);

    // Left Arrow
    fullscreenLeftArrow = createText("<", font, 70, Color::White);
    fullscreenLeftArrow.setPosition(fullscreenText.getPosition().x - 100, fullscreenText.getPosition().y);

    // Right Arrow
    fullscreenRightArrow = createText(">", font, 70, Color::White);
    fullscreenRightArrow.setPosition(fullscreenText.getPosition().x + fullscreenText.getLocalBounds().width + 50, fullscreenText.getPosition().y);

    // Back Button
    backText = createText("Back", font, 50, Color::Yellow);
    backText.setPosition(
        (VideoMode::getDesktopMode().width - backText.getLocalBounds().width) / 2,
        VideoMode::getDesktopMode().height - backText.getLocalBounds().height - 50
    );
}

void SettingsScreen::handleEvent(Event& event, RenderWindow& window, bool& backToStartScreen) {
    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
        Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

        // Toggle Fullscreen
        if (fullscreenLeftArrow.getGlobalBounds().contains(mousePos) || fullscreenRightArrow.getGlobalBounds().contains(mousePos)) {
            isFullscreen = !isFullscreen;
            fullscreenText.setString(isFullscreen ? "Fullscreen: On" : "Fullscreen: Windowed");
            centerText(fullscreenText, VideoMode::getDesktopMode());
            fullscreenText.setPosition(fullscreenText.getPosition().x, fullscreenText.getPosition().y - 50);

            // Apply Resolution
            applyResolution(window, VideoMode(1920, 1080), isFullscreen);
        }

        // Back to Start Screen
        if (backText.getGlobalBounds().contains(mousePos)) {
            backToStartScreen = true;
        }
    }
}

void SettingsScreen::render(RenderWindow& window) {
    window.draw(fullscreenText);
    window.draw(fullscreenLeftArrow);
    window.draw(fullscreenRightArrow);
    window.draw(backText);
}

Text SettingsScreen::createText(const string& content, Font& font, unsigned int size, Color color) {
    Text text(content, font, size);
    text.setFillColor(color);
    return text;
}

void SettingsScreen::centerText(Text& text, const VideoMode& mode) {
    text.setPosition(
        (mode.width - text.getLocalBounds().width) / 2,
        (mode.height - text.getLocalBounds().height) / 2
    );
}

void SettingsScreen::applyResolution(RenderWindow& window, VideoMode& videoMode, bool isFullscreen) {
    window.create(videoMode, "Goblin Siege", isFullscreen ? Style::Fullscreen : Style::Default);
    window.setFramerateLimit(60);
}
