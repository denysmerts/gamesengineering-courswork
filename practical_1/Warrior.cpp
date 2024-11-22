#include "Warrior.h"
#include <iostream>
using namespace sf;
using namespace std;

const int spriteWidth = 192;
const int spriteHeight = 192;

Warrior::Warrior()
    : frameCount(6), currentFrame(0), currentRow(0),
    frameDuration(0.1f), isMoving(false), isFighting(false){
    // Load the sprite sheet texture
    if (!texture.loadFromFile("assets/warrior.png")) {
        cerr << "Error loading texture!" << endl;
    }
    sprite.setTexture(texture);

    // Set the initial crop rectangle (192x192)
    spriteRect = IntRect(0, 0, spriteWidth, spriteHeight);
    sprite.setTextureRect(spriteRect); // Apply the crop to the sprite
}

void Warrior::update() {
    handleInput(); // Handle movement and fight input

    animate(); // Update animation based on state (moving, fighting, standing)

    // Update the position based on movement
    if (isMoving) {
        if (Keyboard::isKeyPressed(Keyboard::W)) sprite.move(0, -5.f); // Move up
        if (Keyboard::isKeyPressed(Keyboard::S)) sprite.move(0, 5.f);  // Move down
        if (Keyboard::isKeyPressed(Keyboard::A)) sprite.move(-5.f, 0); // Move left
        if (Keyboard::isKeyPressed(Keyboard::D)) sprite.move(5.f, 0);  // Move right
    }
}

void Warrior::render(RenderWindow& window) {
    window.draw(sprite); // Draw the warrior sprite
}

void Warrior::handleInput() {
    // Ignore all input if fighting animation is in progress
    if (isFighting) return;

    isMoving = false; // Reset movement state

    // Check for movement input (WASD keys)
    if (Keyboard::isKeyPressed(Keyboard::W) ||
        Keyboard::isKeyPressed(Keyboard::S) ||
        Keyboard::isKeyPressed(Keyboard::A) ||
        Keyboard::isKeyPressed(Keyboard::D)) {
        isMoving = true;
        currentRow = 1; // Running animation row
    }
    else {
        currentRow = 0; // Idle/standing animation row
    }

    // Check for fighting input (F key)
    if (Keyboard::isKeyPressed(Keyboard::F)) {
        isFighting = true;   // Set fighting state
        currentRow = 2;      // Fighting animation row
        currentFrame = 0;    // Start from the first frame
        animationClock.restart(); // Reset animation clock
    }
}


void Warrior::animate() {
    // Update animation only if enough time has passed
    if (animationClock.getElapsedTime().asSeconds() >= frameDuration) {
        if (isFighting) {
            // Fighting animation: Increment frame and check for completion
            currentFrame++;
            if (currentFrame >= frameCount) {
                // Fighting animation complete
                currentFrame = 0;    // Reset frame to start
                isFighting = false; // Exit fighting state
                currentRow = 0;     // Return to idle animation
            }
        }
        else if (isMoving) {
            // Running animation: Loop through frames
            currentFrame = (currentFrame + 1) % frameCount;
        }
        else {
            // Idle animation: Loop through frames in the idle row
            currentFrame = (currentFrame + 1) % frameCount;
            currentRow = 0; // Ensure we're in the idle animation row
        }

        // Update the texture rectangle for the current frame and row
        spriteRect.left = currentFrame * spriteWidth;
        spriteRect.top = currentRow * spriteHeight;
        sprite.setTextureRect(spriteRect);

        // Restart the animation clock
        animationClock.restart();
    }
}


