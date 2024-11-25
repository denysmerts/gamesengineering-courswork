#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "Map.h"  
#include "Warrior.h"
#include "Enemy.h"

using namespace std;
using namespace sf;

enum class GameState {
    StartScreen,
    GameScreen
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

    // Initialize map, warrior, and enemy objects outside the loop
    Map map;
    Warrior warrior;
    Enemy enemy;

    warrior.getSprite().setPosition(200, 200); // Set an initial position for the player

    // Load assets for map and set initial player position
    map.load();

    


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

            // Update phase
            warrior.update(); // Update warrior and pass map for boundaries
            enemy.update();
            map.update();

            // Render phase
            map.render(window);
            enemy.render(window);
            warrior.render(window);
        }

        customCursor.setPosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
        window.draw(customCursor);

        window.display();
    }

    return 0;
}
