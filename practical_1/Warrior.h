#ifndef WARRIOR_H
#define WARRIOR_H

#include <SFML/Audio.hpp>
#include "Character.h"
#include "Map.h"
#include "Enemy.h"

using namespace sf;

class Warrior : public Character {
public:
    // Get the singleton instance
    static Warrior& getInstance();

    // Public methods
    void update(const Map& map) override;
    void update(Enemy& enemy, const Map& map);
    void animate() override;
    void handleInput(const Map& map);
    void initializeSprite();
    void updateHitboxPosition();
    void attack(Enemy& enemy);

    Sprite getSprite() const { return sprite; }
    FloatRect getHitbox() const;

    // Deleted copy constructor and assignment operator to enforce Singleton
    Warrior(const Warrior&) = delete;
    Warrior& operator=(const Warrior&) = delete;

private:
    // Private constructor and destructor for Singleton
    Warrior();
    ~Warrior() = default;

    // Member variables
    bool isMoving;
    bool isFighting;
    bool isFacingLeft;
    bool isAttacking;
    float attackCooldown;

    FloatRect hitbox;
    Clock attackClock;

    SoundBuffer swordSoundBuffer;
    Sound swordSound;

    SoundBuffer walkingSoundBuffer; // Buffer for walking sound
    Sound walkingSound;             // Sound for walking
};

#endif
