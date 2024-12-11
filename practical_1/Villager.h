#ifndef VILLAGER_H
#define VILLAGER_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Character.h"

class Villager : public Character {
public:
    static Villager& getInstance();
    void render(sf::RenderWindow& window, const sf::Vector2f& position);

private:
    Villager(); // Private constructor
    void initializeSprite();
    void animate();

    // Animation-related variables
    int currentFrame;
    int frameCount;
    float frameDuration;
    sf::Clock animationClock;
};

#endif // VILLAGER_H
