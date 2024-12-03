#include "Warrior.h"
#include <iostream>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

Warrior::Warrior()
    : Character(6, 0.1f, 192, 192, 100, 5.0f),
    isMoving(false), isFighting(false), isFacingLeft(false),
    isAttacking(false), attackCooldown(1.0f) {
    initializeSprite();

    if (!swordSoundBuffer.loadFromFile("output/assets/sword-sound.wav")) {
        cerr << "Error loading sword sound!" << endl;
    }
    swordSound.setBuffer(swordSoundBuffer);

    if (!walkingSoundBuffer.loadFromFile("output/assets/walking-sound.wav")) {
        cerr << "Error loading walking sound!" << endl;
    }
    walkingSound.setBuffer(walkingSoundBuffer);
    walkingSound.setLoop(true); // Loop the walking sound
}

void Warrior::initializeSprite() {
    if (!texture.loadFromFile("output/assets/warrior.png")) {
        cerr << "Error loading warrior texture!" << endl;
    }
    sprite.setTexture(texture);
    spriteRect = IntRect(0, 0, spriteWidth, spriteHeight);
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
    bool wasMoving = isMoving;
    isMoving = false;
    isAttacking = false;
    auto position = sprite.getPosition();
    Vector2f nextPosition = position;

    // Keyboard controls: WASD and Arrow keys
    if ((Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) && position.y > 0) {
        nextPosition.y -= 5.f;
        isMoving = true;
    }
    if ((Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)) && position.y + spriteHeight < 1080) {
        nextPosition.y += 5.f;
        isMoving = true;
    }
    if ((Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)) && position.x > 0) {
        nextPosition.x -= 5.f;
        isMoving = true;
        if (!isFacingLeft) {
            sprite.setScale(-1.f, 1.f);
            sprite.setOrigin(spriteWidth, 0);
            isFacingLeft = true;
        }
    }
    if ((Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) && position.x + spriteWidth < 1920) {
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

    // Prevent movement into water tiles
    if (!map.isWaterTile(hitbox.left, hitbox.top)) {
        sprite.setPosition(nextPosition);
    }
    else {
        isMoving = false;
    }

    // Walking sound logic
    if (isMoving && !wasMoving) {
        walkingSound.play();
    }
    else if (!isMoving && wasMoving) {
        walkingSound.stop();
    }

    // Attack action using F key or Mouse Left Button
    if ((Keyboard::isKeyPressed(Keyboard::F) || Mouse::isButtonPressed(Mouse::Left)) && !isFighting) {
        if (attackClock.getElapsedTime().asSeconds() >= attackCooldown) {
            isFighting = true;
            isAttacking = true;
            currentRow = 2;
            currentFrame = 0;
            animationClock.restart();
            attackClock.restart();
            swordSound.play();
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
            cout << "Attack hit the Goblin!" << endl;
            enemy.takeDamage(20); // Deal 20 damage to the Goblin
        }
        else {
            cout << "Attack missed!" << endl;
        }
        isAttacking = false;
    }
}
