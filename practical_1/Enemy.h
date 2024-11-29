#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class Enemy : public Character {
public:

    Enemy();

    void update(const Map& map) override;
    void reset();
    void drawDefeatSprite(RenderWindow& window);
    void moveTowardsPlayer(Vector2f playerPosition);
    void updateFacingDirection(const Vector2f& direction);

    bool checkCollision(const Sprite& warrior);
    bool isDefeated() const;
    bool isActive() const;

    Sprite getSprite() const;
    RectangleShape getHealthBar() const;
    
    

private:
    
    bool active;
    bool isFacingLeft;
    bool defeatAnimationStarted;
    float moveSpeed;
    float healthBarOffsetX;
    float healthBarOffsetY;
    int defeatFrameCount;

    FloatRect hitbox;
    Vector2f hitboxSize;
    Texture defeatTexture;
    Sprite defeatSprite;
    RectangleShape healthBar;
          
};

#endif
