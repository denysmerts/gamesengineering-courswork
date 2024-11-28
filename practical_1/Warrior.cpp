#include "Warrior.h"
#include <iostream>

Warrior::Warrior()
    : Character(6, 0.1f, 192, 192, 100, 5.0f),
    isMoving(false), isFighting(false), isFacingLeft(false),
    isAttacking(false), attackCooldown(1.0f) {
    initializeSprite();
}

void Warrior::initializeSprite() {
    if (!texture.loadFromFile("output/assets/warrior.png")) {
        std::cerr << "Error loading warrior texture!" << std::endl;
    }
    sprite.setTexture(texture);
    spriteRect = sf::IntRect(0, 0, spriteWidth, spriteHeight);
    sprite.setTextureRect(spriteRect);
    sprite.setPosition(200, 200);

    hitbox.width = 64;
    hitbox.height = 64;
    updateHitboxPosition();
}

void Warrior::update(const Map& map) {
    handleInput(map);
    animate();
    updateHitboxPosition();
}

void Warrior::update(Enemy& enemy, const Map& map) {
    handleInput(map);
    animate();
    attack(enemy);
    updateHitboxPosition();
}

void Warrior::handleInput(const Map& map) {
    isMoving = false;
    isAttacking = false;
    auto position = sprite.getPosition();
    sf::Vector2f nextPosition = position;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && position.y > 0) {
        nextPosition.y -= 5.f;
        isMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && position.y + spriteHeight < 1080) {
        nextPosition.y += 5.f;
        isMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && position.x > 0) {
        nextPosition.x -= 5.f;
        isMoving = true;
        if (!isFacingLeft) {
            sprite.setScale(-1.f, 1.f);
            sprite.setOrigin(spriteWidth, 0);
            isFacingLeft = true;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && position.x + spriteWidth < 1920) {
        nextPosition.x += 5.f;
        isMoving = true;
        if (isFacingLeft) {
            sprite.setScale(1.f, 1.f);
            sprite.setOrigin(0, 0);
            isFacingLeft = false;
        }
    }

    hitbox.left = nextPosition.x + 64;
    hitbox.top = nextPosition.y + 64;

    if (!map.isWaterTile(hitbox.left, hitbox.top)) {
        sprite.setPosition(nextPosition);
    }
    else {
        isMoving = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F) && !isFighting) {
        if (attackClock.getElapsedTime().asSeconds() >= attackCooldown) {
            isFighting = true;
            isAttacking = true;
            currentRow = 2;
            currentFrame = 0;
            animationClock.restart();
            attackClock.restart();
        }
    }

    currentRow = isMoving ? 1 : (isFighting ? 2 : 0);
}

void Warrior::updateHitboxPosition() {
    hitbox.left = sprite.getPosition().x + 64;
    hitbox.top = sprite.getPosition().y + 64;
}

sf::FloatRect Warrior::getHitbox() const {
    return hitbox;
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
        else {
            currentFrame = (currentFrame + 1) % frameCount;
        }

        spriteRect.left = currentFrame * spriteWidth;
        spriteRect.top = currentRow * spriteHeight;
        sprite.setTextureRect(spriteRect);

        animationClock.restart();
    }
}

void Warrior::attack(Enemy& enemy) {
    if (isAttacking) {
        // Check collision with the enemy's sprite
        if (sprite.getGlobalBounds().intersects(enemy.getSprite().getGlobalBounds())) {
            std::cout << "Attack hit the Goblin!" << std::endl;
            enemy.takeDamage(20); // Deal 20 damage to the Goblin
        }
        else {
            std::cout << "Attack missed!" << std::endl;
        }
        isAttacking = false;
    }
}