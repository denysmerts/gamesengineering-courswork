#include "Enemy.h"
#include <cmath>
#include <iostream>

using namespace sf;
using namespace std;

Enemy::Enemy()
    : Character(6, 0.1f, 192, 192, 50, 0.5f), moveSpeed(0.5f), isFacingLeft(false) {
    if (!texture.loadFromFile("output/assets/goblin1.png")) {
        cerr << "Error loading enemy texture!" << endl;
    }

    sprite.setTexture(texture);
    spriteRect = IntRect(0, 0, spriteWidth, spriteHeight);
    sprite.setTextureRect(spriteRect);
    sprite.setPosition(600, 300);

    // Initialize health bar
    healthBar.setSize(sf::Vector2f(50.f, 5.f));  // Width based on health
    healthBar.setFillColor(sf::Color::Green);
    healthBar.setPosition(sprite.getPosition().x, sprite.getPosition().y - 10.f);
}

void Enemy::update(const Map& map) {
    animate();

    // Update health bar size and position
    healthBar.setSize(sf::Vector2f(50.f * (static_cast<float>(health) / 50.f), 5.f));
    healthBar.setPosition(sprite.getPosition().x, sprite.getPosition().y - 10.f);
}

void Enemy::reset() {
    // Reset enemy health and position
    health = 50;
    sprite.setPosition(600, 300);
    sprite.setTextureRect(IntRect(0, 0, spriteWidth, spriteHeight));

    // Reset health bar
    healthBar.setSize(Vector2f(50.f, 5.f));
    healthBar.setFillColor(Color::Green);
    healthBar.setPosition(sprite.getPosition().x, sprite.getPosition().y - 10.f);
}

Sprite Enemy::getSprite() const {
    return sprite;
}

void Enemy::moveTowardsPlayer(Vector2f playerPosition) {
    Vector2f enemyPosition = sprite.getPosition();
    Vector2f direction = playerPosition - enemyPosition;

    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0) {
        direction /= length; // Normalize the direction vector
        sprite.move(direction * moveSpeed); // Move the enemy towards the player
        updateFacingDirection(direction); // Update facing direction based on movement
    }
}

void Enemy::updateFacingDirection(const Vector2f& direction) {
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

sf::RectangleShape Enemy::getHealthBar() const {
    return healthBar;
}
