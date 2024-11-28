#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "Map.h"
#include "Warrior.h"
#include "Enemy.h"
#include "CollisionManager.h" // Make sure this is included

using namespace std;
using namespace sf;

enum class GameState {
    StartScreen,
    DialogueScreen,
    GameScreen,
};

int main() {
    RenderWindow window(VideoMode(1920, 1080), "Goblin Siege");
    window.setFramerateLimit(60);

    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("output/assets/bg.png")) {
        cerr << "Failed to load background image!" << endl;
        return -1;
    }

    Sprite background(backgroundTexture);

    Font font;
    if (!font.loadFromFile("output/assets/IrishGrover-Regular.ttf")) {
        cerr << "Failed to load font!" << endl;
        return -1;
    }

    Text title("Goblin Siege", font, 100);
    title.setPosition(120, 50);
    title.setFillColor(Color::White);

    Text startButton("Start Game", font, 40);
    startButton.setPosition(300, 400);
    startButton.setFillColor(Color::White);

    Texture cursorTexture;
    if (!cursorTexture.loadFromFile("output/assets/cursor.png")) {
        cerr << "Failed to load custom cursor!" << endl;
        return -1;
    }

    Sprite customCursor(cursorTexture);
    window.setMouseCursorVisible(false);

    GameState currentState = GameState::StartScreen;

    RectangleShape dialogueBox;
    dialogueBox.setSize(Vector2f(600, 200));
    dialogueBox.setPosition(100, 350);
    dialogueBox.setFillColor(Color(0, 0, 0, 200));
    dialogueBox.setOutlineColor(Color::White);
    dialogueBox.setOutlineThickness(2);

    Text dialogueText("Village Elder: \nBrave knight, the goblins are attacking! \nYou have been trained for this moment, \nDefend our village and defeat the enemy!", font, 30);
    dialogueText.setPosition(120, 380);
    dialogueText.setFillColor(Color::White);

    Text continueText("Press SPACE to continue", font, 20);
    continueText.setPosition(280, 530);
    continueText.setFillColor(Color::Yellow);

    Map map;
    Warrior warrior;
    Enemy enemy;

    warrior.getSprite().setPosition(200, 200); // Set initial player position

    map.load();

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (currentState == GameState::StartScreen && event.type == Event::MouseButtonPressed) {
                Vector2i mousePos = Mouse::getPosition(window);
                if (startButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    currentState = GameState::DialogueScreen;
                }
            }

            if (currentState == GameState::DialogueScreen &&
                event.type == Event::KeyPressed &&
                event.key.code == Keyboard::Space) {
                currentState = GameState::GameScreen;
            }
        }

        window.clear();
        switch (currentState) {
        case GameState::StartScreen:
            window.draw(background);
            window.draw(title);
            window.draw(startButton);
            break;

        case GameState::DialogueScreen:
            window.draw(background);
            window.draw(dialogueBox);
            window.draw(dialogueText);
            window.draw(continueText);
            break;

        case GameState::GameScreen: {
            map.render(window);
            warrior.render(window);
            enemy.render(window);

            window.draw(enemy.getHealthBar()); // Draw enemy's health bar

            warrior.update(enemy, map); // Update warrior with enemy reference
            enemy.update(map);          // Update enemy with map reference

            enemy.moveTowardsPlayer(warrior.getSprite().getPosition());

            // Check collision between the warrior and the goblin
            if (CollisionManager::checkCollision(warrior, enemy)) {
                std::cout << "Collision detected! Warrior and Goblin are interacting." << std::endl;
            }

            if (enemy.isDefeated()) {
                std::cout << "The Enemy has been defeated!" << std::endl;
                enemy.reset(); // Reset the enemy
            }
        }
                                  break;
        }

        customCursor.setPosition(static_cast<float>(Mouse::getPosition(window).x), static_cast<float>(Mouse::getPosition(window).y));
        window.draw(customCursor);

        window.display();
    }

    return 0;
}
