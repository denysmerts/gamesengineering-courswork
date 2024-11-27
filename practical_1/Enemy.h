#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class Enemy : public Character {
public:
    Enemy();
    void update(const Map& map) override;
    Sprite getSprite() const;
    void moveTowardsPlayer(sf::Vector2f playerPosition);
    void updateFacingDirection(const sf::Vector2f& direction);

private:
    float moveSpeed;
    bool isFacingLeft;
};

#endif
