#ifndef WARRIOR_H
#define WARRIOR_H

#include "Character.h"
#include "Map.h"

class Warrior : public Character {
public:
    // Constructor
    Warrior();

    // Update function that incorporates collision detection with the map
    void update(const Map& map) override;

    // Animates the warrior
    void animate() override;

    // Handles player input and movement
    void handleInput(const Map& map);

    // Initializes the warrior sprite
    void initializeSprite();

    // Retrieves the warrior sprite for rendering
    Sprite getSprite() const { return sprite; }

    // Returns the hitbox of the warrior
    FloatRect getHitbox() const;
    void updateHitboxPosition();

private:
    // Movement and state flags
    bool isMoving;
    bool isFighting;
    bool isFacingLeft;

    // Logical hitbox for collision detection
    FloatRect hitbox;
};

#endif
