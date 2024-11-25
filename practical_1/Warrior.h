#ifndef WARRIOR_H
#define WARRIOR_H

#include "Character.h"

class Warrior : public Character {
public:
    Warrior();
    void update() override;
    void animate() override;
    void handleInput();
    void initializeSprite();
    Sprite Warrior::getSprite() const {
        return sprite;
    }

private:
    bool isMoving;
    bool isFighting;
    bool isFacingLeft;
};

#endif