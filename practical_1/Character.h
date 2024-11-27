#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
using namespace sf;

class Map;

class Character {
public:
    Character(int frameCount, float frameDuration, int spriteWidth, int spriteHeight);
    virtual ~Character() = default;

    virtual void update(const Map& map);
    virtual void render(RenderWindow&);
    virtual void animate();
   /* virtual void updatePosition();*/

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
};

#endif