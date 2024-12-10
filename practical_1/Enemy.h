#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include <SFML/Graphics.hpp>

#include <SFML/Audio.hpp>

class Warrior; // Forward declaration


using namespace sf;

class Enemy : public Character {
public:
    static Enemy& getInstance();


    void update(const Map& map) override;

    void reset();
    void drawDefeatSprite(RenderWindow& window);
    void moveTowardsPlayer(Vector2f playerPosition);
    void updateFacingDirection(const Vector2f& direction);

    bool checkCollision(const Sprite& warrior);
    bool isDefeated() const;
    bool isActive() const;

    Sprite getSprite() const;
    RectangleShape getHealthBar() const;


    // New method for dealing damage
    void dealDamage(Warrior& warrior);


    Enemy(const Enemy&) = delete;
    Enemy& operator=(const Enemy&) = delete;

private:
    Enemy();
    ~Enemy() = default;

    bool active;
    bool isFacingLeft;
    bool defeatAnimationStarted;
    float moveSpeed;
    float healthBarOffsetX;
    float healthBarOffsetY;
    int defeatFrameCount;

    FloatRect hitbox;
    Vector2f hitboxSize;
    Texture defeatTexture;
    Sprite defeatSprite;
    RectangleShape healthBar;


    SoundBuffer deathSoundBuffer;
    Sound deathSound;

    // New members for attack functionality
    float attackDamage;
    Clock attackCooldown;
    float attackCooldownTime;
};

#endif

