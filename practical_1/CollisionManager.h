#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "Warrior.h"
#include "Enemy.h"

class CollisionManager {
public:
    static bool checkCollision(const Warrior& warrior, const Enemy& enemy);
};

#endif // COLLISIONMANAGER_H
