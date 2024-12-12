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
#include "Villager.h"
#include "GameOverScreen.h"
#include "WinScreen.h"
#include "StorylineManager.h" // Include StorylineManager

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
    SettingsScreen settingsScreen(font, isFullscreen, VideoMode(1920, 1080), window);
    PauseScreen pauseScreen(font, window);

    vector<Vector2f> villagerPositions = {
        {750, 150},  // Villager 1 position
        {900, 200},  // Villager 2 position
        {1000, 350}  // Villager 3 position
    };

    Map map;
    Warrior& warrior = Warrior::getInstance();
    Enemy& enemy = Enemy::getInstance();
    Villager& villager = Villager::getInstance();
    map.load();

    // Instantiate StorylineManager
    StorylineManager storylineManager(font, window.getSize());

    // Instantiate Game Over and Win screens
    GameOverScreen gameOverScreen(font, window.getSize());
    WinScreen winScreen(font, window.getSize());

    // Game state management
    GameState currentState = GameState::StartScreen;

    // Main game loop
    while (window.isOpen()) {
        // Event handling
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();

            switch (currentState) {
            case GameState::StartScreen: {
                bool startGame = false, openSettings = false, quitGame = false;
                startScreen.handleEvent(event, window, startGame, openSettings, quitGame);
                if (startGame) currentState = GameState::GameScreen;
                if (openSettings) currentState = GameState::SettingsScreen;
                if (quitGame) window.close();
            } break;

            case GameState::SettingsScreen: {
                bool backToStartScreen = false;
                settingsScreen.handleEvent(event, window, backToStartScreen);
                if (backToStartScreen) currentState = GameState::StartScreen;
            } break;

            case GameState::GameScreen: {
                if (storylineManager.isStorylineActive()) {
                    storylineManager.handleInput(event);
                }
                else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                    currentState = GameState::PauseScreen;
                }
            } break;

            case GameState::PauseScreen: {
                bool resumeGame = false, quitToMainMenu = false;
                pauseScreen.handleEvent(event, resumeGame, quitToMainMenu, restartGame);
                if (resumeGame) currentState = GameState::GameScreen;
                if (quitToMainMenu) {
                    warrior.reset();
                    enemy.reset();
                    currentState = GameState::StartScreen;
                }
            } break;

            case GameState::GameOver:
            case GameState::Win: {
                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::R) {
                        warrior.reset();
                        enemy.reset();
                        map.load();
                        currentState = GameState::GameScreen;
                    }
                    else if (event.key.code == Keyboard::Q) {
                        currentState = GameState::StartScreen;
                    }
                }
            } break;

            default: break;
            }
        }

        // Update game logic
        if (currentState == GameState::GameScreen && !storylineManager.isStorylineActive()) {
            warrior.update(enemy, map);
            enemy.update(map);

            if (CollisionManager::checkCollision(warrior, enemy)) {
                cout << "Collision detected! Warrior and Goblin are interacting." << endl;
                if (enemy.isActive()) {
                    enemy.dealDamage(warrior);
                }
            }

            if (warrior.isDefeated()) currentState = GameState::GameOver;
            if (!enemy.isActive() && enemy.isDefeated()) currentState = GameState::Win;

            if (enemy.isActive()) {
                enemy.moveTowardsPlayer(warrior.getSprite().getPosition(), map);
            }
        }

        // Rendering
        window.clear();
        switch (currentState) {
        case GameState::StartScreen:
            startScreen.render(window);
            break;

        case GameState::SettingsScreen:
            settingsScreen.render(window);
            break;

        case GameState::GameScreen:
            map.render(window);
            if (storylineManager.isStorylineActive()) {
                storylineManager.render(window);
            }
            else {
                warrior.render(window);
                if (enemy.isActive()) {
                    enemy.render(window);
                    window.draw(enemy.getHealthBar());
                }

                if (!enemy.isActive() && enemy.isDefeated()) {
                    enemy.drawDefeatSprite(window);
                }

                window.draw(warrior.getHealthBar());
                warrior.renderUI(window);
                for (const auto& position : villagerPositions) {
                    Villager::getInstance().render(window, position);
                }
            }
            break;

        case GameState::PauseScreen:
            pauseScreen.render(window);
            break;

        case GameState::GameOver:
            gameOverScreen.render(window);
            break;

        case GameState::Win:
            winScreen.render(window);
            break;

        default: break;
        }
        window.display();
    }

    return 0;
}

