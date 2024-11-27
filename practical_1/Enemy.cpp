#include "Enemy.h"
#include <cmath>
#include <iostream>

using namespace sf;
using namespace std;

Enemy::Enemy()
    : Character(6, 0.1f, 192, 192), moveSpeed(0.5f), isFacingLeft(false) {
    if (!texture.loadFromFile("output/assets/goblin1.png")) {
        cerr << "Error loading enemy texture!" << endl;
    }

    sprite.setTexture(texture);
    spriteRect = IntRect(0, 0, spriteWidth, spriteHeight);
    sprite.setTextureRect(spriteRect);
    sprite.setPosition(600, 300);
}

void Enemy::update(const Map& map) {
    animate();
   /* updatePosition();*/
}

void Enemy::moveTowardsPlayer(Vector2f playerPosition) {
    Vector2f enemyPosition = sprite.getPosition();
    Vector2f direction = playerPosition - enemyPosition;

    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0) {
        direction /= length;
    }

    sprite.move(direction * moveSpeed);

    updateFacingDirection(direction);
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

Sprite Enemy::getSprite() const {
    return sprite;
}