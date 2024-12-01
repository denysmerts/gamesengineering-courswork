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
    GameScreen,
    PauseScreen,
};

int main() {
    RenderWindow window(VideoMode(1920, 1080), "Goblin Siege");
    window.setFramerateLimit(60);

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

  
    // Instruction text
    Text pressKeyText("Press any key to start", font, 50);
    // Dynamically position the text at the middle bottom of the screen
    FloatRect textBounds = pressKeyText.getLocalBounds();
    pressKeyText.setPosition(
        (window.getSize().x - textBounds.width) / 2,  // Center horizontally
        window.getSize().y - textBounds.height - 40  // Position slightly above the bottom
    );
    pressKeyText.setFillColor(Color::White);


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
    pauseOverlay.setFillColor(Color(0, 0, 0, 150)); // Black with 150 alpha for transparency

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

            // Handle "Press any key to start" in StartScreen
            if (currentState == GameState::StartScreen && event.type == Event::KeyPressed) {
                currentState = GameState::GameScreen;
            }

            // Pause and resume logic
            if (currentState == GameState::GameScreen && event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape) {
                    currentState = GameState::PauseScreen;
                }
            }

            if (currentState == GameState::PauseScreen && event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::R) {
                    currentState = GameState::GameScreen; // Resume game
                }
                else if (event.key.code == Keyboard::Q) {
                    currentState = GameState::StartScreen; // Quit to start screen
                }
            }
        }

        // Clear the window
        window.clear();

        // State-specific rendering
        switch (currentState) {
        case GameState::StartScreen:
            window.draw(background);
            window.draw(pressKeyText); // Display "Press any key to start"
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
            // Draw the current game screen first, then overlay the pause elements
            map.render(window);
            warrior.render(window);
            if (enemy.isActive()) {
                enemy.render(window);
                window.draw(enemy.getHealthBar());
            }
            window.draw(pauseOverlay); // Draw the transparent overlay
            window.draw(pauseTitle);
            window.draw(resumeText);
            window.draw(quitText);
            break;
        }

        // Display the window
        window.display();
    }

    return 0;
}
