#include "Character.h"

Character::Character(int frameCount, float frameDuration, int spriteWidth, int spriteHeight)
    : frameCount(frameCount), frameDuration(frameDuration), spriteWidth(spriteWidth),
    spriteHeight(spriteHeight), currentFrame(0), currentRow(0) {}

void Character::render(RenderWindow& window) {
    window.draw(sprite);
}

void Character::animate() {
    if (animationClock.getElapsedTime().asSeconds() >= frameDuration) {
        currentFrame = (currentFrame + 1) % frameCount;

        spriteRect.left = currentFrame * spriteWidth;
        spriteRect.top = currentRow * spriteHeight;
        sprite.setTextureRect(spriteRect);

        animationClock.restart();
    }
}

void Character::update() {

}

//void character::updateposition() {
//    vector2f position = sprite.getposition();
//    floatrect bounds = sprite.getglobalbounds();
//
//    // ensure the character stays within bounds (example: 800x600 window size)
//    position.x = std::max(0.f, std::min(position.x, 800.f - bounds.width));
//    position.y = std::max(0.f, std::min(position.y, 600.f - bounds.height));
//
//    // apply the clamped position back to the sprite
//    sprite.setposition(position);
//}