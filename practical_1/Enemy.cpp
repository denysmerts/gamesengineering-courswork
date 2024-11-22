#include "Enemy.h"
#include <iostream>

using namespace sf;
using namespace std;

const int spriteWidth = 192;
const int spriteHeight = 192;

Enemy::Enemy()
    : frameCount(6), currentFrame(0), currentRow(0),
    frameDuration(0.1f), isMoving(false), moveSpeed(0.5f), isFacingLeft(false) {

    // Load enemy sprite sheet
    if (!texture.loadFromFile("output/assets/goblin1.png")) {
        cerr << "Error loading enemy texture!" << endl;
    }
    sprite.setTexture(texture);

    // Set initial sprite rectangle
    IntRect spriteRect(0, 0, spriteWidth, spriteHeight);
    sprite.setTextureRect(spriteRect);

    // Set initial position
    sprite.setPosition(600, 300);
}

void Enemy::update() {
    animate();

    // Get window boundaries
    float windowWidth = 800.0f;
    float windowHeight = 600.0f;

    // Get sprite bounds
    FloatRect spriteBounds = sprite.getGlobalBounds();

    // Horizontal boundary check
    if (sprite.getPosition().x < 0)
        sprite.setPosition(0, sprite.getPosition().y);
    if (sprite.getPosition().x + spriteBounds.width > windowWidth)
        sprite.setPosition(windowWidth - spriteBounds.width, sprite.getPosition().y);

    // Vertical boundary check
    if (sprite.getPosition().y < 0)
        sprite.setPosition(sprite.getPosition().x, 0);
    if (sprite.getPosition().y + spriteBounds.height > windowHeight)
        sprite.setPosition(sprite.getPosition().x, windowHeight - spriteBounds.height);
}

void Enemy::render(RenderWindow& window) {
    window.draw(sprite);
}

void Enemy::animate() {
    if (animationClock.getElapsedTime().asSeconds() >= frameDuration) {
        currentFrame = (currentFrame + 1) % frameCount;

        IntRect spriteRect = sprite.getTextureRect();
        spriteRect.left = currentFrame * spriteWidth;
        spriteRect.top = currentRow * spriteHeight;

        sprite.setTextureRect(spriteRect);
        animationClock.restart();
    }
}

void Enemy::moveTowardsPlayer(Vector2f playerPosition) {
    Vector2f enemyPosition = sprite.getPosition();
    Vector2f direction = playerPosition - enemyPosition;

    // Normalize direction
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0) {
        direction /= length;
    }

    // Move enemy
    sprite.move(direction * moveSpeed);

    // Determine animation row based on movement
    if (std::abs(direction.x) > std::abs(direction.y)) {
        if (direction.x > 0) {
            currentRow = 1; // Right movement
            if (isFacingLeft) {
                sprite.setScale(1, 1);
                isFacingLeft = false;
            }
        }
        else {
            currentRow = 2; // Left movement (using same row as right)
            if (!isFacingLeft) {
                sprite.setScale(-1, 1);
                isFacingLeft = true;
            }
        }
    }
    else {
        currentRow = direction.y > 0 ? 3 : 0; // Down or Up movement
        // Reset horizontal scale when moving vertically
        if (isFacingLeft) {
            sprite.setScale(-1, 1);
        }
        else {
            sprite.setScale(1, 1);
        }
    }
}


Sprite Enemy::getSprite() const {
    return sprite;
}