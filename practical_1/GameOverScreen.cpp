#include "GameOverScreen.h"

GameOverScreen::GameOverScreen(const sf::Font& font) {
    gameOverText.setFont(font);
    gameOverText.setString("Game Over!\nPress 'R' to Restart\nPress 'Q' to Quit");
    gameOverText.setCharacterSize(60);
    gameOverText.setFillColor(sf::Color::Red);

    // Center the text
    sf::FloatRect textBounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(textBounds.width / 2, textBounds.height / 2); // Set origin to center
    gameOverText.setPosition(400, 300); // Position at the center of the screen
}

void GameOverScreen::render(sf::RenderWindow& window) {
    window.draw(gameOverText);
}