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
#include "GameOverScreen.h" // Include Game Over Screen
#include "WinScreen.h"      // Include Win Screen

using namespace std;
using namespace sf;

enum class GameState {
    StartScreen,
    SettingsScreen,
    GameScreen,
    PauseScreen,
    GameOver,
    Win,
};

int main() {
    RenderWindow window(VideoMode(1920, 1080), "Goblin Siege");
    window.setFramerateLimit(60);

    // Fullscreen mode flag
    bool isFullscreen = false;
    bool restartGame = false;

    // Assets Loading
    Texture& backgroundTexture = AssetManager::getInstance().getTexture("output/assets/background.png");
    Font& font = AssetManager::getInstance().getFont("output/assets/IrishGrover-Regular.ttf");

    // Initialization
    StartScreen startScreen(font, backgroundTexture);
    SettingsScreen settingsScreen(font, isFullscreen);
    PauseScreen pauseScreen(font, window);

    Map map;
    Warrior& warrior = Warrior::getInstance();
    Enemy& enemy = Enemy::getInstance();
    map.load();

    // Storyline elements
    bool isStorylineActive = true;
    vector<string> storylineTexts = { "Oh no! Our village is under attack! \nPRESS 'ENTER' ",
                                       "We need a brave warrior to save us!\nPRESS 'ENTER'" };
    size_t currentTextIndex = 0;

    RectangleShape textBox(Vector2f(window.getSize().x * 0.8f, 150));
    textBox.setFillColor(Color(0, 0, 0, 200));
    textBox.setPosition(
        (window.getSize().x - textBox.getSize().x) / 2,
        window.getSize().y - textBox.getSize().y - 50);

    Text storylineText("", font, 30);
    storylineText.setFillColor(Color::White);
    storylineText.setPosition(textBox.getPosition().x + 20, textBox.getPosition().y + 20);

    // Instantiate Game Over and Win screens
    GameOverScreen gameOverScreen(font);
    WinScreen winScreen(font);

    // Game state management
    GameState currentState = GameState::StartScreen;

    // Main game loop
    while (window.isOpen()) {
        // Event handling
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

            // Handle pause screen events
            if (currentState == GameState::PauseScreen) {
                bool resumeGame = false, quitToMainMenu = false;
                pauseScreen.handleEvent(event, resumeGame, quitToMainMenu, restartGame); // Ensure this matches the method signature
                if (resumeGame) currentState = GameState::GameScreen;
                if (quitToMainMenu) {
                    warrior.reset();
                    enemy.reset();
                    currentState = GameState::StartScreen;
                }
            }

            // Handle game over and win events
            if (currentState == GameState::GameOver || currentState == GameState::Win) {
                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::R) {
                        warrior.reset(); // Reset warrior state
                        enemy.reset();   // Reset enemy state
                        map.load();      // Reload map if necessary
                        currentState = GameState::StartScreen; // Or directly back to game screen based on your design
                    }
                    else if (event.key.code == Keyboard::Q) {
                        window.close();
                    }
                }
            }
        }

        // Update game logic
        if (currentState == GameState::GameScreen && !isStorylineActive) {
            warrior.update(enemy, map);
            enemy.update(map);

            if (CollisionManager::checkCollision(warrior, enemy)) {
                cout << "Collision detected! Warrior and Goblin are interacting." << endl;
                if (enemy.isActive()) {
                    enemy.dealDamage(warrior); // Assuming warrior takes damage here
                }
            }

            // Check for defeat or victory conditions using health points
            if (warrior.isDefeated()) { // Check if the warrior is defeated based on health
                currentState = GameState::GameOver;
            }
            else if (!enemy.isActive() && enemy.isDefeated()) { // Check if the enemy is defeated
                currentState = GameState::Win;
            }

            // Move the enemy towards the player
            if (enemy.isActive()) {
                enemy.moveTowardsPlayer(warrior.getSprite().getPosition());
            }
        }

        // Rendering
        window.clear();
        switch (currentState) {
        case GameState::StartScreen:
            startScreen.render(window);
            break;

        case GameState::GameScreen:
            map.render(window);
            if (enemy.isActive()) {
                enemy.render(window);
                window.draw(enemy.getHealthBar());
            }

            // Render defeat sprite if enemy is defeated
            if (!enemy.isActive() && enemy.isDefeated()) {
                enemy.drawDefeatSprite(window);
            }

            warrior.render(window);
            window.draw(warrior.getHealthBar());

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

        case GameState::GameOver:
            gameOverScreen.render(window); // Render the Game Over screen
            break;

        case GameState::Win:
            winScreen.render(window);       // Render the Win screen
            break;
        }

        window.display();
    }

    return 0;
}