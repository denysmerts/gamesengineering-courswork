#include "WinScreen.h"
using namespace sf;

WinScreen::WinScreen(const Font& font, const Vector2u& windowSize) {
    winText.setFont(font);
    winText.setString("You have saved us all!!!\nPress 'R' to Restart\nPress 'Q' to Quit");
    winText.setCharacterSize(60);
    winText.setFillColor(Color::Green);

    // Center the text
    FloatRect textBounds = winText.getLocalBounds();
    winText.setOrigin(textBounds.left + textBounds.width / 2.0f,
        textBounds.top + textBounds.height / 2.0f);

    winText.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
}

void WinScreen::render(RenderWindow& window) {
    window.draw(winText);
}
