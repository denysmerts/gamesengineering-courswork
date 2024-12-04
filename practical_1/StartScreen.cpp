#include "StartScreen.h"

using namespace sf;

StartScreen::StartScreen(Font& font, Texture& backgroundTexture)
    : font(font) {
    // Set up background
    background.setTexture(backgroundTexture);
    auto windowSize = VideoMode::getDesktopMode();
    background.setScale(
        static_cast<float>(windowSize.width) / backgroundTexture.getSize().x,
        static_cast<float>(windowSize.height) / backgroundTexture.getSize().y
    );

    // Set up "Press SPACE to start" text
    pressKeyText.setFont(font);
    pressKeyText.setString("Press SPACE to start");
    pressKeyText.setCharacterSize(50);
    pressKeyText.setFillColor(Color::White);
    pressKeyText.setPosition(
        (windowSize.width - pressKeyText.getLocalBounds().width) / 2,
        (windowSize.height - pressKeyText.getLocalBounds().height) / 2
    );

    // Set up Settings button
    settingsButton.setFont(font);
    settingsButton.setString("Settings");
    settingsButton.setCharacterSize(40);
    settingsButton.setFillColor(Color::White);
    settingsButton.setPosition(
        (windowSize.width - settingsButton.getLocalBounds().width) / 2,
        windowSize.height - 150 // 150 pixels from the bottom
    );

    // Set up Quit button
    quitGameButton.setFont(font);
    quitGameButton.setString("Quit Game");
    quitGameButton.setCharacterSize(40);
    quitGameButton.setFillColor(Color::Red);
    quitGameButton.setPosition(
        (windowSize.width - quitGameButton.getLocalBounds().width) / 2,
        settingsButton.getPosition().y + settingsButton.getLocalBounds().height + 20 // Spaced 20 pixels below the settings button
    );
}

void StartScreen::handleEvent(Event& event, RenderWindow& window, bool& startGame, bool& openSettings, bool& quitGame) {
    if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
        startGame = true;
    }
    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
        Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
        if (settingsButton.getGlobalBounds().contains(mousePos)) {
            openSettings = true;
        }
        if (quitGameButton.getGlobalBounds().contains(mousePos)) {
            quitGame = true;
        }
    }
}

void StartScreen::render(RenderWindow& window) {
    window.draw(background);
    window.draw(pressKeyText);
    window.draw(settingsButton);
    window.draw(quitGameButton);
}
