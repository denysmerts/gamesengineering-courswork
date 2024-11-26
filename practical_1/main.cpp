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

   
 // ------------------------------ INITIALIZING -------------------------// 
   
    Map map;
    Warrior warrior;
    Enemy enemy;

 // ------------------------------ INITIALIZING -------------------------// 
    warrior.getSprite().setPosition(200, 200); // Set initial player position

    map.load();
 
    // Main game loop
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

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
                /*levelClock.restart();*/
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

        //----------------------------- RENDERING ----------------------------//

            map.render(window);
            warrior.render(window);
            enemy.render(window);

         //----------------------------- RENDERING ----------------------------//

         //----------------------------- UPDATING ----------------------------//

            warrior.update();
            enemy.update();
            map.update();

         //----------------------------- UPDATING ----------------------------//

            enemy.moveTowardsPlayer(warrior.getSprite().getPosition());           
    
        }
        }

        // Custom cursor
        customCursor.setPosition(static_cast<float>(Mouse::getPosition(window).x), static_cast<float>(Mouse::getPosition(window).y));
            
        window.draw(customCursor);

        window.display();
    }

    return 0;
}
