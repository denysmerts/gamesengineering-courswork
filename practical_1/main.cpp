#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
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
    RenderWindow window(VideoMode(800, 600), "Goblin Siege");
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

    // Create an instance of Warrior
    Warrior warrior;

    // Load level 1 background
    Texture level1Texture;
    if (!level1Texture.loadFromFile("output/assets/bg.png")) {
        cerr << "Failed to load level 1 background!" << endl;
        return -1;
    }

    Sprite level1Background(level1Texture);

    // Create an enemy
    Enemy enemy;

    // Timer setup for Level 1
    Clock levelClock;
    int levelDuration = 60; // Level duration in seconds
    Text timerText("", font, 30);
    timerText.setPosition(650, 10);
    timerText.setFillColor(Color::White);



    // Main game loop
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            // ESC key handling for pausing
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                if (currentState == GameState::GameScreen) {
                    currentState = GameState::PauseScreen;
                }
                else if (currentState == GameState::PauseScreen) {
                    currentState = GameState::GameScreen;
                    levelClock.restart(); // Restart clock on resume to avoid time drift
                }
            }

            // Handle pause screen inputs
            if (currentState == GameState::PauseScreen) {
                if (event.type == Event::KeyPressed) {
                    switch (event.key.code) {
                    case Keyboard::R:  // Resume game
                        currentState = GameState::GameScreen;
                        levelClock.restart(); // Restart clock on resume to avoid time drift
                        break;
                    case Keyboard::Q:  // Quit game
                        window.close();
                        break;
                    }
                }
            }

            // Handle mouse click on the "Start" button if we are on the StartScreen
            if (currentState == GameState::StartScreen && event.type == Event::MouseButtonPressed) {
                Vector2i mousePos = Mouse::getPosition(window);
                if (startButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    cout << "Moving to dialogue screen..." << endl;
                    currentState = GameState::DialogueScreen;
                }
            }

            // Handle dialogue screen progression
            if (currentState == GameState::DialogueScreen &&
                event.type == Event::KeyPressed &&
                event.key.code == Keyboard::Space) {
                currentState = GameState::GameScreen;
                levelClock.restart(); // Start timer when entering game screen
            }

            // Handle restart from game over screen
            if (currentState == GameState::GameOverScreen &&
                event.type == Event::KeyPressed &&
                event.key.code == Keyboard::R) {
                currentState = GameState::StartScreen; // Restart game from start screen
            }
        }

        // Get the mouse position relative to the window
        Vector2i mousePos = Mouse::getPosition(window);

        // Render the current screen based on the game state
        window.clear();

        if (currentState == GameState::StartScreen) {
            window.draw(background);
            window.draw(title);
            window.draw(startButton);
        }
        else if (currentState == GameState::DialogueScreen) {
            window.draw(background);
            window.draw(dialogueBox);
            window.draw(dialogueText);
            window.draw(continueText);
        }
        else if (currentState == GameState::GameScreen) {
            window.draw(level1Background);

            // Update and render warrior
            warrior.update();
            restrictToWindow(warrior.getSprite(), window);
            warrior.render(window);

            // Update and render enemy
            enemy.update();
            enemy.moveTowardsPlayer(warrior.getSprite().getPosition());
            restrictToWindow(enemy.getSprite(), window);
            enemy.render(window);

            // Update timer text based on elapsed time
            int elapsedSeconds = static_cast<int>(levelClock.getElapsedTime().asSeconds());
            int remainingTime = max(0, levelDuration - elapsedSeconds);
            timerText.setString("Time: " + std::to_string(remainingTime));
            window.draw(timerText);

            Text levelTitle("Level 1", font, 50);
            levelTitle.setPosition(250, 0);
            levelTitle.setFillColor(Color::White);
            window.draw(levelTitle);

            // Check if time has run out and handle accordingly (e.g., end level)
            if (remainingTime <= 0) {
                cout << "Time's up! Level over." << endl;
                currentState = GameState::GameOverScreen;
            }
        }
        else if (currentState == GameState::PauseScreen) {
            RectangleShape overlay(Vector2f(800, 600));
            overlay.setFillColor(Color(0, 0, 0, 150));
            window.draw(overlay);

            window.draw(pauseTitle);
            window.draw(resumeText);
            window.draw(quitText);
        }
        else if (currentState == GameState::GameOverScreen) {
            RectangleShape overlay(Vector2f(800, 600));
            overlay.setFillColor(Color(0, 0, 0, 150));
            window.draw(overlay);

            window.draw(gameOverText);
            window.draw(restartText);
        }

        customCursor.setPosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
        window.draw(customCursor);

        window.display();
    }

    return 0;
}