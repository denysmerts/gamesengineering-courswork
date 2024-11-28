#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class Enemy : public Character {
public:
    Enemy();
    void update(const Map& map) override;
    void reset(); // Function to reset enemy (no longer used for respawn)
    Sprite getSprite() const;
    void moveTowardsPlayer(sf::Vector2f playerPosition);
    void updateFacingDirection(const sf::Vector2f& direction);
    bool isDefeated() const;       // Function to check if the enemy is defeated
    sf::RectangleShape getHealthBar() const; // Function to get the health bar
    bool isActive() const;        // Checks if the enemy is active

private:
    float moveSpeed;
    bool isFacingLeft;
    sf::RectangleShape healthBar; // Health bar UI
    bool active;                  // Tracks if the enemy is active
};

#endif
