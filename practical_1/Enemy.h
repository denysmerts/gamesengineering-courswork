#ifndef ENEMY_H
#define ENEMY_H
#include <SFML/Graphics.hpp>
using namespace sf;

class Enemy {
public:
    Enemy();
    void update();
    void render(RenderWindow& window);
    Sprite getSprite() const;
    void moveTowardsPlayer(Vector2f playerPosition);
    bool isFacingLeft = false;

private:
    Texture texture;
    Sprite sprite;
    Clock animationClock;

    int frameCount;
    int currentFrame;
    int currentRow;
    float frameDuration;
    bool isMoving;
    float moveSpeed;

    void animate();
};
#endif