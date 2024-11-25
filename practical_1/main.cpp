#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "Map.h"
#include "Warrior.h"
#include "Enemy.h"

using namespace std;
using namespace sf;

// Boundary restriction function
void restrictToWindow(Sprite& sprite, const RenderWindow& window) {
    FloatRect bounds = sprite.getGlobalBounds();
    Vector2u windowSize = window.getSize();

    sprite.setPosition(
        max(0.f, min(sprite.getPosition().x, windowSize.x - bounds.width)),
        max(0.f, min(sprite.getPosition().y, windowSize.y - bounds.height))
    );
}

enum class GameState {
    StartScreen,
    DialogueScreen,
    GameScreen,
    PauseScreen,
    GameOverScreen
};

int main() {
    RenderWindow window(VideoMode(1920, 1080), "Goblin Siege");
    window.setFramerateLimit(60);

    // Load the background texture
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("output/assets/bg.png")) {
        cerr << "Failed to load background image!" << endl;
        return -1;
    }
    Sprite background(backgroundTexture);

    // Load the font for the text
    Font font;
    if (!font.loadFromFile("output/assets/IrishGrover-Regular.ttf")) {
        cerr << "Failed to load font!" << endl;
        return -1;
    }

    // Create the title text
    Text title("Goblin Siege", font, 100);
    title.setPosition(120, 50);
    title.setFillColor(Color::White);

    // Create the "Start" button
    Text startButton("Start Game", font, 40);
    startButton.setPosition(300, 400);
    startButton.setFillColor(Color::White);

    // Load the custom cursor image
    Texture cursorTexture;
    if (!cursorTexture.loadFromFile("output/assets/cursor.png")) {
        cerr << "Failed to load custom cursor!" << endl;
        return -1;
    }
    Sprite customCursor(cursorTexture);

    // Hide the default system cursor
    window.setMouseCursorVisible(false);

    // Set the initial game state
    GameState currentState = GameState::StartScreen;

    // Dialogue box setup
    RectangleShape dialogueBox;
    dialogueBox.setSize(Vector2f(600, 200));
    dialogueBox.setPosition(100, 350);
    dialogueBox.setFillColor(Color(0, 0, 0, 200)); // Semi-transparent black
    dialogueBox.setOutlineColor(Color::White);
    dialogueBox.setOutlineThickness(2);

    Text dialogueText("Village Elder: \nBrave knight, the goblins are attacking! \nYou have been trained for this moment, \nDefend our village and defeat the enemy!", font, 30);
    dialogueText.setPosition(120, 380);
    dialogueText.setFillColor(Color::White);

    Text continueText("Press SPACE to continue", font, 20);
    continueText.setPosition(280, 530);
    continueText.setFillColor(Color::Yellow);

    // Pause screen elements
    Text pauseTitle("GAME PAUSED", font, 80);
    pauseTitle.setPosition(150, 200);
    pauseTitle.setFillColor(Color::White);

    Text resumeText("Press R to Resume", font, 40);
    resumeText.setPosition(250, 350);
    resumeText.setFillColor(Color::Yellow);

    Text quitText("Press Q to Quit", font, 40);
    quitText.setPosition(250, 420);
    quitText.setFillColor(Color::Yellow);

    // Game Over screen elements
    Text gameOverText("Game Over! You have failed us.", font, 50);
    gameOverText.setPosition(100, 250);
    gameOverText.setFillColor(Color::Red);

    Text restartText("Press R to Restart", font, 30);
    restartText.setPosition(250, 350);
    restartText.setFillColor(Color::Yellow);

    // Initialize map, warrior, and enemy objects
    Map map;
    map.load();

    Warrior warrior;
    warrior.getSprite().setPosition(200, 200); // Set initial player position

    Enemy enemy;

    // Timer setup for Level 1
    Clock levelClock;
    int levelDuration = 60; // Level duration in seconds
    Text timerText("", font, 30);
    timerText.setPosition(650, 10);
    timerText.setFillColor(Color::White);

    // Load level 1 background
    Texture level1Texture;
    if (!level1Texture.loadFromFile("output/assets/bg.png")) {
        cerr << "Failed to load level 1 background!" << endl;
        return -1;
    }
    Sprite level1Background(level1Texture);

    // Main game loop
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape && currentState == GameState::GameScreen) {
                    currentState = GameState::PauseScreen;
                }
                else if (event.key.code == Keyboard::Escape && currentState == GameState::PauseScreen) {
                    currentState = GameState::GameScreen;
                    levelClock.restart();
                }
            }

            // Handle mouse click on "Start" button
            if (currentState == GameState::StartScreen && event.type == Event::MouseButtonPressed) {
                Vector2i mousePos = Mouse::getPosition(window);
                if (startButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    currentState = GameState::DialogueScreen;
                }
            }

            // Dialogue screen progression
            if (currentState == GameState::DialogueScreen &&
                event.type == Event::KeyPressed &&
                event.key.code == Keyboard::Space) {
                currentState = GameState::GameScreen;
                levelClock.restart();
            }

            // Handle restart from Game Over screen
            if (currentState == GameState::GameOverScreen &&
                event.type == Event::KeyPressed &&
                event.key.code == Keyboard::R) {
                currentState = GameState::StartScreen;
            }
        }

        // Render
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
           /* window.draw(level1Background);*/

            // Update and render player
            warrior.update();
            restrictToWindow(warrior.getSprite(), window);
            warrior.render(window);

            // Update enemy
            enemy.moveTowardsPlayer(warrior.getSprite().getPosition());
            restrictToWindow(enemy.getSprite(), window);
            enemy.render(window);

            // Update timer
            int elapsedSeconds = static_cast<int>(levelClock.getElapsedTime().asSeconds());
            int remainingTime = max(0, levelDuration - elapsedSeconds);
            timerText.setString("Time: " + to_string(remainingTime));
            window.draw(timerText);

            if (remainingTime <= 0) {
                currentState = GameState::GameOverScreen;
            }
            break;
        }

        case GameState::PauseScreen:
            window.draw(pauseTitle);
            window.draw(resumeText);
            window.draw(quitText);
            break;

        case GameState::GameOverScreen:
            window.draw(gameOverText);
            window.draw(restartText);
            break;
        }

        // Custom cursor
        customCursor.setPosition(static_cast<float>(Mouse::getPosition(window).x),
            static_cast<float>(Mouse::getPosition(window).y));
        window.draw(customCursor);

        window.display();
    }

    return 0;
}
