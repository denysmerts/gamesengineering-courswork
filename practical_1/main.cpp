#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "Map.h"
#include "Warrior.h"
#include "Enemy.h"
#include "CollisionManager.h"
#include "AssetManager.h"
#include "SettingsScreen.h"
#include "StartScreen.h"
#include "PauseScreen.h"

using namespace std;
using namespace sf;

enum class GameState {
    StartScreen,
    SettingsScreen,
    GameScreen,
    PauseScreen,
};

int main() {
    RenderWindow window(VideoMode(1920, 1080), "Goblin Siege");
    window.setFramerateLimit(60);

    // Fullscreen mode flag
    bool isFullscreen = false;

    // --------------------- ASSETS LOADING  ----------------------------//

    Texture& backgroundTexture = AssetManager::getInstance().getTexture("output/assets/background.png");

    Font& font = AssetManager::getInstance().getFont("output/assets/IrishGrover-Regular.ttf");

    // --------------------- ASSETS LOADING  ----------------------------//
   

    // --------------------- INITIALIZATION ----------------------------//

    StartScreen startScreen(font, backgroundTexture);
    SettingsScreen settingsScreen(font, isFullscreen);
    PauseScreen pauseScreen(font, window);

    Map map;
    Warrior& warrior = Warrior::getInstance();
    Enemy& enemy = Enemy::getInstance();

    // --------------------- INITIALIZATION ----------------------------//
     
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
                bool startGame = false, openSettings = false, quitGame = false;
                startScreen.handleEvent(event, window, startGame, openSettings, quitGame);
                if (startGame) currentState = GameState::GameScreen;
                if (openSettings) currentState = GameState::SettingsScreen;
                if (quitGame) window.close();
            }

            // Settings screen interactions
            if (currentState == GameState::SettingsScreen) {
                bool backToStartScreen = false;
                settingsScreen.handleEvent(event, window, backToStartScreen);
                if (backToStartScreen) {
                    currentState = GameState::StartScreen;
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

            // Pause screen interactions
            if (currentState == GameState::GameScreen && !isStorylineActive && event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape) currentState = GameState::PauseScreen;
            }

            if (currentState == GameState::PauseScreen) {
                bool resumeGame = false, quitToMainMenu = false;
                pauseScreen.handleEvent(event, resumeGame, quitToMainMenu);
                if (resumeGame) currentState = GameState::GameScreen;
                if (quitToMainMenu) currentState = GameState::StartScreen;
            }
        }

        // Rendering logic
        window.clear();
        switch (currentState) {
        case GameState::StartScreen:
            startScreen.render(window);
            break;

        case GameState::GameScreen:
            map.render(window);
            warrior.render(window);
            if (enemy.isActive()) {
                enemy.render(window);
                window.draw(enemy.getHealthBar());
                enemy.moveTowardsPlayer(warrior.getSprite().getPosition());
            }

            // Render defeat sprite if enemy is defeated
            if (!enemy.isActive() && enemy.isDefeated()) {
                enemy.drawDefeatSprite(window);
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
            pauseScreen.render(window);
            break;

        case GameState::SettingsScreen:
            settingsScreen.render(window);
            break;
        }

        window.display();
    }

    return 0;
}
