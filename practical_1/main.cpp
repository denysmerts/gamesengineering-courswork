#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "Map.h"
#include "Warrior.h"
#include "Enemy.h"
#include "CollisionManager.h"

using namespace std;
using namespace sf;

enum class GameState {
    StartScreen,
    SettingsScreen,
    GameScreen,
    PauseScreen,
};

void applyResolution(RenderWindow& window, VideoMode& videoMode, bool isFullscreen) {
    window.create(videoMode, "Goblin Siege", isFullscreen ? Style::Fullscreen : Style::Default);
    window.setFramerateLimit(60);
}

int main() {
    RenderWindow window(VideoMode(1920, 1080), "Goblin Siege");
    window.setFramerateLimit(60);

    // Default fullscreen mode
    bool isFullscreen = false;

    // Load background texture
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("output/assets/background.png")) {
        cerr << "Failed to load background image!" << endl;
        return -1;
    }
    Sprite background(backgroundTexture);

    Vector2u windowSize = window.getSize();
    Vector2u textureSize = backgroundTexture.getSize();

    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    background.setScale(scaleX, scaleY);

    // Load font
    Font font;
    if (!font.loadFromFile("output/assets/IrishGrover-Regular.ttf")) {
        cerr << "Failed to load font!" << endl;
        return -1;
    }

    // Start screen elements

    // Center "Press SPACE to start" in the middle of the screen
    Text pressKeyText("Press SPACE to start", font, 50);
    FloatRect pressKeyBounds = pressKeyText.getLocalBounds();
    pressKeyText.setPosition(
        (window.getSize().x - pressKeyBounds.width) / 2, // Center horizontally
        (window.getSize().y - pressKeyBounds.height) / 2 // Center vertically
    );
    pressKeyText.setFillColor(Color::White);

    // Dynamically position "Quit Game" button at the bottom of the screen
    Text quitGameButton("Quit Game", font, 40);
    FloatRect quitGameBounds = quitGameButton.getLocalBounds();
    quitGameButton.setPosition(
        (window.getSize().x - quitGameBounds.width) / 2, // Center horizontally
        window.getSize().y - quitGameBounds.height - 50 // 50 pixels above the bottom
    );
    quitGameButton.setFillColor(Color::Red);

    // Dynamically position "Settings" button above "Quit Game"
    Text settingsButton("Settings", font, 40);
    FloatRect settingsBounds = settingsButton.getLocalBounds();
    settingsButton.setPosition(
        (window.getSize().x - settingsBounds.width) / 2, // Center horizontally
        quitGameButton.getPosition().y - settingsBounds.height - 20 // 20 pixels above "Quit Game"
    );
    settingsButton.setFillColor(Color::White);

    // Settings screen elements
    Text fullscreenText("Fullscreen: Windowed", font, 50);
    FloatRect fullscreenBounds = fullscreenText.getLocalBounds();
    fullscreenText.setPosition(
        (window.getSize().x - fullscreenBounds.width) / 2,
        (window.getSize().y - fullscreenBounds.height) / 2 - 50
    );
    fullscreenText.setFillColor(Color::White);

    Text fullscreenLeftArrow("<", font, 70);
    fullscreenLeftArrow.setPosition(
        fullscreenText.getPosition().x - 100, // Position to the left of fullscreen text
        fullscreenText.getPosition().y
    );
    fullscreenLeftArrow.setFillColor(Color::White);

    Text fullscreenRightArrow(">", font, 70);
    fullscreenRightArrow.setPosition(
        fullscreenText.getPosition().x + fullscreenBounds.width + 50, // Position to the right of fullscreen text
        fullscreenText.getPosition().y
    );
    fullscreenRightArrow.setFillColor(Color::White);

    // Dynamically position "Back" button to center bottom
    Text backText("Back", font, 50);
    FloatRect backBounds = backText.getLocalBounds();
    backText.setPosition(
        (window.getSize().x - backBounds.width) / 2, // Center horizontally
        window.getSize().y - backBounds.height - 50 // Slightly above the bottom
    );
    backText.setFillColor(Color::Yellow);

    // Pause screen elements
    Text pauseTitle("Game Paused", font, 80);
    pauseTitle.setPosition(650, 200);
    pauseTitle.setFillColor(Color::White);

    Text resumeText("Press R to Resume", font, 40);
    resumeText.setPosition(750, 400);
    resumeText.setFillColor(Color::Yellow);

    Text quitText("Press Q to Quit", font, 40);
    quitText.setPosition(750, 500);
    quitText.setFillColor(Color::Yellow);

    // Semi-transparent background for pause screen
    RectangleShape pauseOverlay(Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
    pauseOverlay.setFillColor(Color(0, 0, 0, 150));

    // Initialize map, warrior, and enemy
    Map map;
    Warrior warrior;
    Enemy enemy;

    warrior.getSprite().setPosition(200, 200);
    map.load();

    // Game state management
    GameState currentState = GameState::StartScreen;

    // Main game loop
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            // Handle "Press SPACE to start", "Settings", or "Quit Game" on StartScreen
            if (currentState == GameState::StartScreen) {
                if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
                    currentState = GameState::GameScreen;
                }
                else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                    if (settingsButton.getGlobalBounds().contains(mousePos)) {
                        currentState = GameState::SettingsScreen;
                    }

                    if (quitGameButton.getGlobalBounds().contains(mousePos)) {
                        window.close();
                    }
                }
            }

            // Settings screen interaction
            if (currentState == GameState::SettingsScreen) {
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                    // Toggle fullscreen mode
                    if (fullscreenLeftArrow.getGlobalBounds().contains(mousePos) ||
                        fullscreenRightArrow.getGlobalBounds().contains(mousePos)) {
                        isFullscreen = !isFullscreen;
                        fullscreenText.setString(isFullscreen ? "Fullscreen: On" : "Fullscreen: Windowed");
                        applyResolution(window, VideoMode(1920, 1080), isFullscreen);
                    }

                    // Back to Start Screen
                    if (backText.getGlobalBounds().contains(mousePos)) {
                        currentState = GameState::StartScreen;
                    }
                }
            }

            // Pause and resume logic
            if (currentState == GameState::GameScreen && event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape) {
                    currentState = GameState::PauseScreen;
                }
            }

            if (currentState == GameState::PauseScreen && event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::R) {
                    currentState = GameState::GameScreen;
                }
                else if (event.key.code == Keyboard::Q) {
                    currentState = GameState::StartScreen;
                }
            }
        }

        // Clear the window
        window.clear();

        // State-specific rendering
        switch (currentState) {
        case GameState::StartScreen:
            window.draw(background);
            window.draw(pressKeyText);
            window.draw(settingsButton);
            window.draw(quitGameButton);
            break;

        case GameState::GameScreen:
            map.render(window);
            warrior.render(window);
            if (enemy.isActive()) {
                enemy.render(window);
                window.draw(enemy.getHealthBar());
                enemy.moveTowardsPlayer(warrior.getSprite().getPosition());
            }
            warrior.update(enemy, map);
            enemy.update(map);

            if (CollisionManager::checkCollision(warrior, enemy)) {
                cout << "Collision detected! Warrior and Goblin are interacting." << endl;
            }
            break;

        case GameState::PauseScreen:
            map.render(window);
            warrior.render(window);
            if (enemy.isActive()) {
                enemy.render(window);
                window.draw(enemy.getHealthBar());
            }
            window.draw(pauseOverlay);
            window.draw(pauseTitle);
            window.draw(resumeText);
            window.draw(quitText);
            break;

        case GameState::SettingsScreen:
            window.draw(fullscreenText);
            window.draw(fullscreenLeftArrow);
            window.draw(fullscreenRightArrow);
            window.draw(backText);
            break;
        }

        // Display the window
        window.display();
    }

    return 0;
}
