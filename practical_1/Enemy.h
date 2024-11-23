#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class Enemy : public Character {
public:
    Enemy();
    void update() override;                      
    void moveTowardsPlayer(sf::Vector2f playerPosition);
    Sprite getSprite() const;

private:
    float moveSpeed;
    bool isFacingLeft;

    void updateFacingDirection(const sf::Vector2f& direction);
};

#endif
