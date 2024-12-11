#include "Villager.h"
#include <iostream>
#include "AssetManager.h"

Villager& Villager::getInstance() {
    static Villager instance;
    return instance;
}

// Constructor
Villager::Villager()
    : Character(6, 0.1f, 192, 192, 100, 5.0f),
    currentFrame(0), frameCount(6), frameDuration(0.1f) { // Set animation parameters
    initializeSprite();
}

void Villager::initializeSprite() {
    // Load texture using AssetManager
    texture = AssetManager::getInstance().getTexture("output/assets/villager.png");
    sprite.setTexture(texture);
    spriteRect = IntRect(0, 0, spriteWidth, spriteHeight); // Start at frame 0, row 0
    sprite.setTextureRect(spriteRect);
    sprite.setPosition(800, 200);
}

void Villager::animate() {
    if (animationClock.getElapsedTime().asSeconds() >= frameDuration) {
        // Move to the next frame
        currentFrame = (currentFrame + 1) % frameCount;

        // Update texture rectangle to the new frame (row 0)
        spriteRect.left = currentFrame * spriteWidth; // Shift horizontally for each frame
        spriteRect.top = 0;                          // Row 0 for animation
        sprite.setTextureRect(spriteRect);

        animationClock.restart();
    }
}

void Villager::render(sf::RenderWindow& window, const sf::Vector2f& position) {
    animate(); // Update animation
    sprite.setPosition(position); // Set Villager's position dynamically
    window.draw(sprite);          // Draw Villager at the specified position
}

