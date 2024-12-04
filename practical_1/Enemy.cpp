#include "Enemy.h"
#include "AssetManager.h"
#include <cmath>
#include <iostream>

#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

// Singleton instance getter
Enemy& Enemy::getInstance() {
    static Enemy instance; // Guaranteed to be destroyed and instantiated on first use
    return instance;
}

// Private constructor
Enemy::Enemy()
    : Character(6, 0.1f, 192, 192, 50, 0.5f),
    moveSpeed(0.5f),
    isFacingLeft(false),
    active(true),
    defeatAnimationStarted(false),
    defeatFrameCount(0),
    healthBarOffsetX(64.f),
    healthBarOffsetY(40.f) {

    // Load textures using AssetManager
    Texture& goblinTexture = AssetManager::getInstance().getTexture("output/assets/goblin1.png");
    Texture& defeatTex = AssetManager::getInstance().getTexture("output/assets/dead.png");

    deathSoundBuffer = AssetManager::getInstance().getSoundBuffer("output/assets/goblin-death.wav");
    deathSound.setBuffer(deathSoundBuffer);

    // Set up enemy texture and sprite
    sprite.setTexture(goblinTexture);
    spriteRect = IntRect(0, 0, spriteWidth, spriteHeight);
    sprite.setTextureRect(spriteRect);
    sprite.setPosition(600, 300);

    // Set up defeat sprite
    defeatTexture = defeatTex;
    defeatSprite.setTexture(defeatTexture);

    // Initialize health bar
    healthBar.setSize(Vector2f(50.f, 5.f));
    healthBar.setFillColor(Color::Green);
    healthBar.setPosition(sprite.getPosition().x + healthBarOffsetX, sprite.getPosition().y);
}

void Enemy::update(const Map& map) {
    if (!active && !defeatAnimationStarted) return;

    if (health <= 0) {
        if (!defeatAnimationStarted) {
            defeatSprite.setTexture(defeatTexture);
            defeatSprite.setTextureRect(IntRect(1, 1, 128, 128)); // Start with the first frame
            defeatSprite.setScale(sprite.getScale());
            defeatSprite.setPosition(sprite.getPosition());
            defeatAnimationStarted = true;

            deathSound.play();
        }

        // Animate defeat sprite
        if (defeatAnimationStarted) {
            int frame = defeatFrameCount / 10; // Adjust speed of animation (10 frames per defeat frame)
            if (frame < 14) {
                // Calculate frame coordinates
                int frameX = (frame % 7) * 128;
                int frameY = (frame / 7) * 128;
                defeatSprite.setTextureRect(IntRect(frameX, frameY, 128, 128));
            }
            else {
                active = false; // End animation after all frames
            }
            defeatFrameCount++;
        }

        return;
    }

    // Normal updates for active enemy
    animate();
    healthBar.setSize(Vector2f(50.f * (static_cast<float>(health) / 50.f), 5.f));
    Vector2f spritePos = sprite.getPosition();
    healthBar.setPosition(spritePos.x + healthBarOffsetX, spritePos.y + healthBarOffsetY);
}

void Enemy::drawDefeatSprite(RenderWindow& window) {
    if (defeatAnimationStarted) {
        // Set the texture rect to the 7th frame (first row, 6th column)
        defeatSprite.setTextureRect(IntRect(768, 0, 128, 128)); // Frame 7 (0-based indexing)
        defeatSprite.setScale(sprite.getScale());
        defeatSprite.setPosition(sprite.getPosition());
        window.draw(defeatSprite);
    }
}



void Enemy::reset() {
    // Reset enemy state
    health = 50;
    active = true;
    defeatAnimationStarted = false;
    defeatFrameCount = 0;
    sprite.setPosition(600, 300);
}

bool Enemy::checkCollision(const Sprite& warrior) {
    return sprite.getGlobalBounds().intersects(warrior.getGlobalBounds());
}

void Enemy::moveTowardsPlayer(Vector2f playerPosition) {
    if (!active) return;

    Vector2f enemyPosition = sprite.getPosition();
    Vector2f direction = playerPosition - enemyPosition;
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length > 50.0f) {
        direction /= length; // Normalize the direction vector
        sprite.move(direction * moveSpeed);
        updateFacingDirection(direction);
    }
}

void Enemy::updateFacingDirection(const Vector2f& direction) {
    if (!active) return;

    if (abs(direction.x) > abs(direction.y)) {
        if (direction.x > 0 && isFacingLeft) {
            sprite.setScale(1, 1);
            isFacingLeft = false;
        }
        else if (direction.x < 0 && !isFacingLeft) {
            sprite.setScale(-1, 1);
            isFacingLeft = true;
        }
    }

    currentRow = (abs(direction.y) > abs(direction.x)) ?
        (direction.y > 0 ? 3 : 0) :
        (direction.x > 0 ? 1 : 2);
    spriteRect.top = currentRow * spriteHeight;
}

bool Enemy::isDefeated() const {
    return health <= 0;
}

Sprite Enemy::getSprite() const {
    return sprite;
}

RectangleShape Enemy::getHealthBar() const {
    return healthBar;
}

bool Enemy::isActive() const {
    return active;
}

