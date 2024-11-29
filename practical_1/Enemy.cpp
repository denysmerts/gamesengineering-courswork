#include "Enemy.h"
#include <cmath>
#include <iostream>

using namespace sf;
using namespace std;

Enemy::Enemy()
    : Character(6, 0.1f, 192, 192, 50, 0.5f), moveSpeed(0.5f), isFacingLeft(false), active(true) {
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
    if (!active) return; // Skip update if inactive

    animate();

    if (health <= 0) {
        active = false; // Deactivate the enemy when health reaches 0
        return;
    }

    // Update health bar size and position
    healthBar.setSize(sf::Vector2f(50.f * (static_cast<float>(health) / 50.f), 5.f));
    healthBar.setPosition(sprite.getPosition().x, sprite.getPosition().y - 10.f);
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
    return active; // Return whether the enemy is active
}

void Enemy::moveTowardsPlayer(Vector2f playerPosition) {
    if (!active) return; // Skip movement if inactive

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
    if (!active) return; // Skip direction update if inactive

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
