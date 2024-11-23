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


    bool isMoving;
    bool isFighting;
};

#endif
