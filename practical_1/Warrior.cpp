#include "Warrior.h"
#include <iostream>

using namespace std;

Warrior::Warrior()
    : Character(6, 0.1f, 192, 192), isMoving(false), isFighting(false) {
    initializeSprite();
}

void Warrior::initializeSprite() {
    if (!texture.loadFromFile("output/assets/warrior.png")) {
        cerr << "Error loading warrior texture!" << endl;
    }
    sprite.setTexture(texture);
    spriteRect = IntRect(0, 0, spriteWidth, spriteHeight);
    sprite.setTextureRect(spriteRect);
}

void Warrior::handleInput() {
    isMoving = false;

    if (Keyboard::isKeyPressed(Keyboard::W)) {
        sprite.move(0, -5.f);
        isMoving = true;
        currentRow = 1;
    }
    if (Keyboard::isKeyPressed(Keyboard::S)) {
        sprite.move(0, 5.f);
        isMoving = true;
        currentRow = 1;
    }
    if (Keyboard::isKeyPressed(Keyboard::A)) {
        sprite.move(-5.f, 0);
        isMoving = true;
        currentRow = 1;
    }
    if (Keyboard::isKeyPressed(Keyboard::D)) {
        sprite.move(5.f, 0);
        isMoving = true;
        currentRow = 1;
    }

   
    if (Keyboard::isKeyPressed(Keyboard::F) && !isFighting) {
        isFighting = true;
        currentRow = 2;
        currentFrame = 0; 
        animationClock.restart();
    }

    if (!isMoving && !isFighting) {
        currentRow = 0; 
    }
}

void Warrior::update() {
    handleInput(); 
    animate();     

    updatePosition();
}

void Warrior::animate() {
    if (animationClock.getElapsedTime().asSeconds() >= frameDuration) {
        if (isFighting) {
            
            currentFrame++;
            if (currentFrame >= frameCount) {
                
                currentFrame = 0;
                isFighting = false;
                currentRow = 0;
            }
        }
        else if (isMoving) {
            
            currentFrame = (currentFrame + 1) % frameCount;
        }
        else {
           
            currentFrame = (currentFrame + 1) % frameCount;
        }

        spriteRect.left = currentFrame * spriteWidth;
        spriteRect.top = currentRow * spriteHeight;
        sprite.setTextureRect(spriteRect);

        animationClock.restart();
    }
}
