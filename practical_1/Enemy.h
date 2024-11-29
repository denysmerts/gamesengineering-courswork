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
    void drawDefeatSprite(sf::RenderWindow& window);
    
    Sprite getSprite() const;
    void moveTowardsPlayer(sf::Vector2f playerPosition);
    void updateFacingDirection(const sf::Vector2f& direction);

    bool checkCollision(const sf::Sprite& warrior);
    bool isDefeated() const;       
    sf::RectangleShape getHealthBar() const; 
    bool isActive() const;        

private:
    float moveSpeed;
    bool isFacingLeft;
    sf::RectangleShape healthBar; 
    bool active;                  
    
    sf::Texture defeatTexture;
    sf::Sprite defeatSprite;
    bool defeatAnimationStarted;
    int defeatFrameCount;
};
#endif