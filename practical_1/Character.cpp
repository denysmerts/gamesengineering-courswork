#include "Character.h"
#include "Map.h"

Character::Character(int frameCount, float frameDuration, int spriteWidth, int spriteHeight, int hp, float speed)
    : frameCount(frameCount), frameDuration(frameDuration), spriteWidth(spriteWidth),
    spriteHeight(spriteHeight), currentFrame(0), currentRow(0), health(hp), movementSpeed(speed) {
    // Initialize other members here if needed
}

void Character::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Character::animate() {
    if (animationClock.getElapsedTime().asSeconds() >= frameDuration) {
        currentFrame = (currentFrame + 1) % frameCount;

        spriteRect.left = currentFrame * spriteWidth;
        spriteRect.top = currentRow * spriteHeight;
        sprite.setTextureRect(spriteRect);

        animationClock.restart();
    }
}

void Character::update(const Map& map) {
    // Implementation here
}

int Character::getHealth() const {
    return health;
}

void Character::setHealth(int hp) {
    health = hp;
}

sf::Sprite Character::getSprite() const {
    return sprite;
}

void Character::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0; // Prevent negative health
}