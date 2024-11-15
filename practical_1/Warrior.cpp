#include "Warrior.h"
#include <iostream>
using namespace sf;
using namespace std;

Warrior::Warrior()
    : frameCount(6), currentFrame(0), currentRow(0),
    frameDuration(0.1f), isMoving(false), isFighting(false) {
    // Load the sprite sheet texture
    if (!texture.loadFromFile("assets/warrior.png")) {
        cerr << "Error loading texture!" << endl;
    }
    sprite.setTexture(texture);

    // Set the initial crop rectangle (192x192)
    spriteRect = IntRect(0, 0, 192, 192);
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
    isMoving = false; // Reset movement state
    isFighting = false; // Reset fighting state

    // Check for movement input (WASD keys)
    if (Keyboard::isKeyPressed(Keyboard::W) ||
        Keyboard::isKeyPressed(Keyboard::S) ||
        Keyboard::isKeyPressed(Keyboard::A) ||
        Keyboard::isKeyPressed(Keyboard::D)) {
        isMoving = true;
        currentRow = 1; // Set to running animation
    }
    else {
        currentRow = 0; // Set to standing animation
    }

    // Check for fighting input (F key)
    if (Keyboard::isKeyPressed(Keyboard::F)) {
        isFighting = true; // Set to fighting animation
        currentRow = 2;    // Set the row to fighting (assumed to be row 2 in the sprite sheet)
    }
}

void Warrior::animate() {
    // Update animation based on the elapsed time
    if (animationClock.getElapsedTime().asSeconds() >= frameDuration) {
        // Move to the next frame
        currentFrame = (currentFrame + 1) % frameCount; // Loop back to the first frame after the last frame

        // Update the texture rectangle for the current frame and row
        spriteRect.left = currentFrame * 192;    // Update the "left" part of the rect to show the next frame
        spriteRect.top = currentRow * 192;       // Update the "top" part of the rect to switch between rows (standing, running, fighting)
        sprite.setTextureRect(spriteRect);       // Apply the updated rectangle to the sprite

        // Reset the animation clock
        animationClock.restart();
    }
}
