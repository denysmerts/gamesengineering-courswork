#include "Warrior.h"
#include <iostream>

using namespace std;

Warrior::Warrior()
    : Character(6, 0.1f, 192, 192), isMoving(false), isFighting(false), isFacingLeft(false) {
    initializeSprite();
}

void Warrior::initializeSprite() {
    if (!texture.loadFromFile("output/assets/warrior.png")) {
        cerr << "Error loading warrior texture!" << endl;
    }
    sprite.setTexture(texture);
    spriteRect = IntRect(0, 0, spriteWidth, spriteHeight);
    sprite.setTextureRect(spriteRect);
    sprite.setPosition(200, 200);

    // Initialize hitbox (64x64)
    hitbox.width = 64;
    hitbox.height = 64;
    updateHitboxPosition(); // Sync hitbox with initial sprite position
}

void Warrior::update(const Map& map) {
    handleInput(map);
    animate();
    updateHitboxPosition(); // Sync hitbox with sprite position
}

void Warrior::handleInput(const Map& map) {
    isMoving = false; // Reset movement state
    auto position = sprite.getPosition();
    Vector2f nextPosition = position;

    // Handle movement keys
    if (Keyboard::isKeyPressed(Keyboard::W) && position.y > 0) {
        nextPosition.y -= 5.f;
        isMoving = true;
    }
    if (Keyboard::isKeyPressed(Keyboard::S) && position.y + spriteHeight < 1080) {
        nextPosition.y += 5.f;
        isMoving = true;
    }
    if (Keyboard::isKeyPressed(Keyboard::A) && position.x > 0) {
        nextPosition.x -= 5.f;
        isMoving = true;

        if (!isFacingLeft) {
            sprite.setScale(-1.f, 1.f); // Flip sprite horizontally
            sprite.setOrigin(spriteWidth, 0);
            isFacingLeft = true;
        }
    }
    if (Keyboard::isKeyPressed(Keyboard::D) && position.x + spriteWidth < 1920) {
        nextPosition.x += 5.f;
        isMoving = true;

        if (isFacingLeft) {
            sprite.setScale(1.f, 1.f); // Reset sprite flip
            sprite.setOrigin(0, 0);
            isFacingLeft = false;
        }
    }

    // Check collision using the hitbox
    hitbox.left = nextPosition.x + 64; // Offset to align hitbox within sprite
    hitbox.top = nextPosition.y + 64;

    if (!map.isWaterTile(hitbox.left, hitbox.top)) {
        sprite.setPosition(nextPosition); // Update position only if valid
    }
    else {
        isMoving = false; // Block movement if water tile
    }

    // Handle fighting input
    if (Keyboard::isKeyPressed(Keyboard::F) && !isFighting) {
        isFighting = true;
        currentRow = 2;      // Fighting animation row
        currentFrame = 0;    // Start fighting animation from the beginning
        animationClock.restart();
    }

    // Determine animation row for movement or idle
    if (isMoving) {
        currentRow = 1;  // Running animation row
    }
    else if (!isFighting) {
        currentRow = 0;  // Idle animation row
    }
}

void Warrior::updateHitboxPosition() {
    // Align the hitbox with the center of the sprite (192x192 -> 64x64)
    hitbox.left = sprite.getPosition().x + 64; // Offset X
    hitbox.top = sprite.getPosition().y + 64;  // Offset Y
}

sf::FloatRect Warrior::getHitbox() const {
    return hitbox; // Return the logical hitbox
}

void Warrior::animate() {
    if (animationClock.getElapsedTime().asSeconds() >= frameDuration) {
        if (isFighting) {
            currentFrame++;
            if (currentFrame >= frameCount) {
                currentFrame = 0;
                isFighting = false;
                currentRow = 0;
            }
        }
        else if (isMoving) {
            currentFrame = (currentFrame + 1) % frameCount;
        }
        else {
            currentFrame = (currentFrame + 1) % frameCount;
        }

        spriteRect.left = currentFrame * spriteWidth;
        spriteRect.top = currentRow * spriteHeight;
        sprite.setTextureRect(spriteRect);

        animationClock.restart();
    }
}
