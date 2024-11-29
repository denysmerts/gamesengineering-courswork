#include "Enemy.h"
#include <cmath>
#include <iostream>
using namespace sf;
using namespace std;

Enemy::Enemy()
    : Character(6, 0.1f, 192, 192, 50, 0.5f),
    moveSpeed(0.5f),
    isFacingLeft(false),
    active(true),
    defeatAnimationStarted(false),
    defeatFrameCount(0) {
    if (!texture.loadFromFile("output/assets/goblin1.png")) {
        cerr << "Error loading enemy texture!" << endl;
    }
    sprite.setTexture(texture);
    spriteRect = IntRect(0, 0, spriteWidth, spriteHeight);
    sprite.setTextureRect(spriteRect);
    sprite.setPosition(600, 300);

    // Load defeat texture
    if (!defeatTexture.loadFromFile("output/assets/dead.png")) {
        cerr << "Error loading enemy defeat texture!" << endl;
    }
    defeatSprite.setTexture(defeatTexture);

    // Initialize health bar
    healthBar.setSize(sf::Vector2f(50.f, 5.f));
    healthBar.setFillColor(sf::Color::Green);
    healthBar.setPosition(sprite.getPosition().x, sprite.getPosition().y - 0.5f);

}

void Enemy::update(const Map& map) {
    if (!active) return;

    if (health <= 0) {
        if (!defeatAnimationStarted) {
            defeatSprite.setTextureRect(IntRect(128, 0, 128, 128));
            defeatSprite.setScale(sprite.getScale());
            defeatSprite.setPosition(sprite.getPosition());
            defeatAnimationStarted = true;
        }

        defeatFrameCount++;
        if (defeatFrameCount > 60) {
            active = false;
        }
        return;
    }

    animate();

    // Update health bar size and position
    healthBar.setSize(sf::Vector2f(50.f * (static_cast<float>(health) / 50.f), 5.f));
    healthBar.setPosition(sprite.getPosition().x, sprite.getPosition().y - 0.5f);

}

void Enemy::reset() {
    // Reset enemy state
    health = 50;
    active = true;
    defeatAnimationStarted = false;
    defeatFrameCount = 0;
    sprite.setPosition(600, 300);
}

bool Enemy::checkCollision(const sf::Sprite& warrior) {
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

sf::RectangleShape Enemy::getHealthBar() const {
    return healthBar;
}

bool Enemy::isActive() const {
    return active;
}

void Enemy::drawDefeatSprite(sf::RenderWindow& window) {
    if (defeatAnimationStarted && !active) {
        window.draw(defeatSprite);
    }
}