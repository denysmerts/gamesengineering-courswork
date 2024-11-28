#include "CollisionManager.h"
#include <SFML/Graphics.hpp>

// Check if the Warrior's hitbox intersects with the Enemy's sprite
bool CollisionManager::checkCollision(const Warrior& warrior, const Enemy& enemy) {
    return warrior.getHitbox().intersects(enemy.getSprite().getGlobalBounds());
}
