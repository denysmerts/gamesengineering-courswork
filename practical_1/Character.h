#ifndef CHARACTER_H
#define CHARACTER_H
#include <SFML/Graphics.hpp>
using namespace sf;

class Map;

class Character {
public:
    Character(int frameCount, float frameDuration, int spriteWidth, int spriteHeight, int hp, float speed);
    virtual ~Character() = default;

    virtual void update(const Map& map); 
    virtual void render(RenderWindow& window);
    virtual void animate();

    int getHealth() const;
    void setHealth(int hp);
    Sprite getSprite() const;
    void takeDamage(int damage);

protected:
    Texture texture;
    Sprite sprite;
    IntRect spriteRect;
    Clock animationClock;

    int frameCount;
    int currentFrame;
    int currentRow;
    float frameDuration;

    int spriteWidth;
    int spriteHeight;
    int health;
    float movementSpeed;
};

#endif