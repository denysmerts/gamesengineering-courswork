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
        std::max(0.f, std::min(sprite.getPosition().x, windowSize.x - bounds.width)),
        std::max(0.f, std::min(sprite.getPosition().y, windowSize.y - bounds.height))
    );
}

enum class GameState {
    StartScreen,
    GameScreen
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

    // Main game loop
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            // Handle mouse click on the "Start" button if we are on the StartScreen
            if (currentState == GameState::StartScreen && event.type == Event::MouseButtonPressed) {
                Vector2i mousePos = Mouse::getPosition(window);
                if (startButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    cout << "Starting the game..." << endl;
                    currentState = GameState::GameScreen; // Switch to the game screen
                }
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
        else if (currentState == GameState::GameScreen) {
            window.draw(level1Background);

            // Update the warrior in the game screen
            warrior.update();
            // Restrict warrior to window boundaries
            restrictToWindow(warrior.getSprite(), window);
            warrior.render(window);

            // Update and render enemy
            enemy.update();
            enemy.moveTowardsPlayer(warrior.getSprite().getPosition());
            // Restrict enemy to window boundaries
            restrictToWindow(enemy.getSprite(), window);
            enemy.render(window);

            Text levelTitle("Level 1", font, 50);
            levelTitle.setPosition(250, 0);
            levelTitle.setFillColor(Color::White);
            window.draw(levelTitle);
        }

        // Update and draw the custom cursor
        customCursor.setPosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
        window.draw(customCursor);

        window.display();
    }

    return 0;
}