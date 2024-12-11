#include "Enemy.h"
#include "AssetManager.h"

#include "Warrior.h"
#include <cmath>
#include <iostream>


using namespace sf;
using namespace std;

// Singleton instance getter
Enemy& Enemy::getInstance() {

    static Enemy instance;

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

    healthBarOffsetY(40.f),
    attackDamage(5.f),
    attackCooldownTime(1.0f) {


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

            defeatSprite.setTextureRect(IntRect(1, 1, 128, 128));

            defeatSprite.setScale(sprite.getScale());
            defeatSprite.setPosition(sprite.getPosition());
            defeatAnimationStarted = true;

            deathSound.play();
        }

        // Animate defeat sprite
        if (defeatAnimationStarted) {

            int frame = defeatFrameCount / 10;
            if (frame < 14) {

                int frameX = (frame % 7) * 128;
                int frameY = (frame / 7) * 128;
                defeatSprite.setTextureRect(IntRect(frameX, frameY, 128, 128));
            }
            else {

                active = false;

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
    health = 50;
    active = true;
    defeatAnimationStarted = false;
    defeatFrameCount = 0;
    sprite.setPosition(600, 300);
}

bool Enemy::checkCollision(const Sprite& warrior) {
    return sprite.getGlobalBounds().intersects(warrior.getGlobalBounds());
}

void Enemy::moveTowardsPlayer(Vector2f playerPosition, Map& map)
{
    if (!active) return;

    Vector2f enemyPosition = sprite.getPosition();
    Vector2f direction = playerPosition - enemyPosition;
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length > 50.0f) {
        direction /= length; // Normalize the direction vector
        Vector2f nextPosition = enemyPosition + (direction * moveSpeed);

        // Check if the next position is a valid tile
        FloatRect nextHitbox(nextPosition.x, nextPosition.y, spriteWidth, spriteHeight);
        if (!map.isWaterTile(nextHitbox.left, nextHitbox.top)) {
            sprite.setPosition(nextPosition); // Move enemy only if it's not a boundary
            updateFacingDirection(direction);
        }
    }
}


void Enemy::updateFacingDirection(const Vector2f& direction) {
    if (!active) return;

    // Horizontal movement: Flip sprite horizontally
    if (abs(direction.x) > abs(direction.y)) {
        if (direction.x > 0 && isFacingLeft) {
            // Face right
            sprite.setScale(1, 1);
            sprite.setOrigin(0, 0);
            isFacingLeft = false;
        }
        else if (direction.x < 0 && !isFacingLeft) {
            // Face left
            sprite.setScale(-1, 1);
            sprite.setOrigin(spriteWidth, 0);
            isFacingLeft = true;
        }

        currentRow = 2; // Always row 2 for horizontal movement
    }
    else {
        // Vertical movement
        if (direction.y > 0) {
            currentRow = 3; // Row 3 for downward movement
        }
        else {
            currentRow = 4; // Row 4 for attacking upwards
        }
    }

    // Update the sprite's texture rectangle
    spriteRect.top = currentRow * spriteHeight;
    sprite.setTextureRect(spriteRect);
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


// New method for dealing damage
void Enemy::dealDamage(Warrior& warrior) {
    if (attackCooldown.getElapsedTime().asSeconds() >= attackCooldownTime) {
        warrior.takeDamage(attackDamage);
        attackCooldown.restart();
        cout << "Enemy dealt " << attackDamage << " damage to Warrior!" << endl;
    }
}

