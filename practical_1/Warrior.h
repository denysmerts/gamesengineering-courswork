#ifndef WARRIOR_H
#define WARRIOR_H

#include "Character.h"
#include "Map.h"
#include "Enemy.h"

class Warrior : public Character {
public:
    Warrior();
    void update(const Map& map) override;
    void update(Enemy& enemy, const Map& map);
    void animate() override;
    void handleInput(const Map& map);
    void initializeSprite();
    sf::Sprite getSprite() const { return sprite; }
    sf::FloatRect getHitbox() const;
    void updateHitboxPosition();
    void attack(Enemy& enemy);

private:
    bool isMoving;
    bool isFighting;
    bool isFacingLeft;
    bool isAttacking;
    sf::FloatRect hitbox;
    sf::Clock attackClock;
    float attackCooldown;
};

#endif