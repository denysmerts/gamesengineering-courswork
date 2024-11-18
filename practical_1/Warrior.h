#ifndef WARRIOR_H
#define WARRIOR_H

#include <SFML/Graphics.hpp>

using namespace sf;

class Warrior {
public:
    Warrior();  // Constructor

    void update();        // Update warrior state
    void render(sf::RenderWindow& window);  // Render the warrior
    Sprite Warrior::getSprite() const {
        return sprite; // Return actual sprite instead of empty sprite
    }

private:
    void handleInput();    // Handle keyboard input
    void animate();        // Update the animation frames

    Texture texture;  // Texture for the sprite sheet
    Sprite sprite;    // The sprite displaying the warrior
    IntRect spriteRect; // Rectangle to crop the sprite sheet (frame selection)
    Clock animationClock; // Timer for animation speed

    int frameCount;       // Total frames in a row of the sprite sheet
    int currentFrame;     // Current frame for animation
    int currentRow;       // Current row of the sprite sheet (standing, running, fighting)
    int previousRow;      // Store previous row before fight (running or standing)

    float frameDuration;  // Duration for each frame (seconds)
    bool isMoving;        // Check if the warrior is moving
    bool isFighting;      // Check if the warrior is fighting
   
};

#endif
