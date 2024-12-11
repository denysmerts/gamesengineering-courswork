#include "GameOverScreen.h"

using namespace sf;

GameOverScreen::GameOverScreen(const Font& font, const Vector2u& windowSize) {
    gameOverText.setFont(font);
    gameOverText.setString("Game Over!\nPress 'R' to Restart\nPress 'Q' to Quit");
    gameOverText.setCharacterSize(80);
    gameOverText.setFillColor(Color::Red);

    // Center the text
    FloatRect textBounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(textBounds.left + textBounds.width / 2.0f,
        textBounds.top + textBounds.height / 2.0f);

    gameOverText.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
}

void GameOverScreen::render(RenderWindow& window) {
    window.draw(gameOverText);
}