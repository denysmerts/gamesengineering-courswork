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

    // Fullscreen mode flag
    bool isFullscreen = false;

    // Load background texture
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("output/assets/background.png")) {
        cerr << "Failed to load background image!" << endl;
        return -1;
    }
    Sprite background(backgroundTexture);
    background.setScale(
        static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
        static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y
    );

    // Load font
    Font font;
    if (!font.loadFromFile("output/assets/IrishGrover-Regular.ttf")) {
        cerr << "Failed to load font!" << endl;
        return -1;
    }

    // Start screen elements
    Text pressKeyText("Press SPACE to start", font, 50);
    pressKeyText.setPosition(
        (window.getSize().x - pressKeyText.getLocalBounds().width) / 2,
        (window.getSize().y - pressKeyText.getLocalBounds().height) / 2
    );
    pressKeyText.setFillColor(Color::White);

    Text quitGameButton("Quit Game", font, 40);
    quitGameButton.setPosition(
        (window.getSize().x - quitGameButton.getLocalBounds().width) / 2,
        window.getSize().y - quitGameButton.getLocalBounds().height - 50
    );
    quitGameButton.setFillColor(Color::Red);

    Text settingsButton("Settings", font, 40);
    settingsButton.setPosition(
        (window.getSize().x - settingsButton.getLocalBounds().width) / 2,
        quitGameButton.getPosition().y - settingsButton.getLocalBounds().height - 20
    );
    settingsButton.setFillColor(Color::White);

    // Settings screen elements
    Text fullscreenText("Fullscreen: Windowed", font, 50);
    fullscreenText.setPosition(
        (window.getSize().x - fullscreenText.getLocalBounds().width) / 2,
        (window.getSize().y - fullscreenText.getLocalBounds().height) / 2 - 50
    );
    fullscreenText.setFillColor(Color::White);

    Text fullscreenLeftArrow("<", font, 70);
    fullscreenLeftArrow.setPosition(fullscreenText.getPosition().x - 100, fullscreenText.getPosition().y);
    fullscreenLeftArrow.setFillColor(Color::White);

    Text fullscreenRightArrow(">", font, 70);
    fullscreenRightArrow.setPosition(fullscreenText.getPosition().x + fullscreenText.getLocalBounds().width + 50, fullscreenText.getPosition().y);
    fullscreenRightArrow.setFillColor(Color::White);

    Text backText("Back", font, 50);
    backText.setPosition(
        (window.getSize().x - backText.getLocalBounds().width) / 2,
        window.getSize().y - backText.getLocalBounds().height - 50
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

    RectangleShape pauseOverlay(Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
    pauseOverlay.setFillColor(Color(0, 0, 0, 150));

    // Initialize map, warrior, and enemy
    Map map;
    Warrior warrior;
    Enemy enemy;

    warrior.getSprite().setPosition(200, 200);
    map.load();

    // Storyline elements
    bool isStorylineActive = true;
    vector<string> storylineTexts = { "Oh no! Our village is under attack!", "We need a brave warrior to save us!" };
    size_t currentTextIndex = 0;

    RectangleShape textBox(Vector2f(window.getSize().x * 0.8f, 150));
    textBox.setFillColor(Color(0, 0, 0, 200));
    textBox.setPosition(
        (window.getSize().x - textBox.getSize().x) / 2,
        window.getSize().y - textBox.getSize().y - 50
    );

    Text storylineText("", font, 30);
    storylineText.setFillColor(Color::White);
    storylineText.setPosition(textBox.getPosition().x + 20, textBox.getPosition().y + 20);

    // Game state management
    GameState currentState = GameState::StartScreen;

    // Main game loop
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();

            // Start screen interactions
            if (currentState == GameState::StartScreen) {
                if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
                    currentState = GameState::GameScreen;
                }
                else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    if (settingsButton.getGlobalBounds().contains(mousePos)) currentState = GameState::SettingsScreen;
                    if (quitGameButton.getGlobalBounds().contains(mousePos)) window.close();
                }
            }

            // Settings screen interactions
            if (currentState == GameState::SettingsScreen) {
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    if (fullscreenLeftArrow.getGlobalBounds().contains(mousePos) || fullscreenRightArrow.getGlobalBounds().contains(mousePos)) {
                        isFullscreen = !isFullscreen;
                        fullscreenText.setString(isFullscreen ? "Fullscreen: On" : "Fullscreen: Windowed");
                        applyResolution(window, VideoMode(1920, 1080), isFullscreen);
                    }
                    if (backText.getGlobalBounds().contains(mousePos)) currentState = GameState::StartScreen;
                }
            }

            // Storyline interaction
            if (currentState == GameState::GameScreen && isStorylineActive) {
                if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                    currentTextIndex++;
                    if (currentTextIndex >= storylineTexts.size()) isStorylineActive = false;
                    else storylineText.setString(storylineTexts[currentTextIndex]);
                }
            }

            // Pause and resume logic
            if (currentState == GameState::GameScreen && !isStorylineActive && event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape) currentState = GameState::PauseScreen;
            }

            if (currentState == GameState::PauseScreen && event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::R) currentState = GameState::GameScreen;
                else if (event.key.code == Keyboard::Q) currentState = GameState::StartScreen;
            }
        }

        // Rendering logic
        window.clear();
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

            if (!isStorylineActive) {
                warrior.update(enemy, map);
                enemy.update(map);
            }

            if (CollisionManager::checkCollision(warrior, enemy)) {
                cout << "Collision detected! Warrior and Goblin are interacting." << endl;
            }

            if (isStorylineActive) {
                window.draw(textBox);
                storylineText.setString(storylineTexts[currentTextIndex]);
                window.draw(storylineText);
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

        window.display();
    }

    return 0;
}
