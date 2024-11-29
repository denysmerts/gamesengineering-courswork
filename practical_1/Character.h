#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>

class Map;

class Character {
public:
    Character(int frameCount, float frameDuration, int spriteWidth, int spriteHeight, int hp, float speed);
    virtual ~Character() = default;

    virtual void update(const Map& map); // Ensure this is virtual
    virtual void render(sf::RenderWindow& window);
    virtual void animate();

    int getHealth() const;
    void setHealth(int hp);
    sf::Sprite getSprite() const;
    void takeDamage(int damage);

protected:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::IntRect spriteRect;
    sf::Clock animationClock;

    int frameCount;
    int currentFrame;
    int currentRow;
    float frameDuration;

    int spriteWidth;
    int spriteHeight;
    int health;
    float movementSpeed;
};

#endif // CHARACTER_H