#include "WinScreen.h"

WinScreen::WinScreen(const sf::Font& font) {
    winText.setFont(font);
    winText.setString("You have saved us all!!!\nPress 'R' to Restart\nPress 'Q' to Quit");
    winText.setCharacterSize(60);
    winText.setFillColor(sf::Color::Green);

    // Center the text
    sf::FloatRect textBounds = winText.getLocalBounds();
    winText.setOrigin(textBounds.width / 2, textBounds.height / 2); // Set origin to center
    winText.setPosition(400, 300); // Position at the center of the screen
}

void WinScreen::render(sf::RenderWindow& window) {
    window.draw(winText);
}