#include "PauseScreen.h"

using namespace sf;

PauseScreen::PauseScreen(Font& font, RenderWindow& window)
    : font(font) {
    // Overlay with transparency
    overlay.setSize(Vector2f(window.getSize().x, window.getSize().y));
    overlay.setFillColor(Color(0, 0, 0, 100)); // Adjust transparency (alpha = 100)

    // Pause title
    pauseTitle.setFont(font);
    pauseTitle.setString("Game Paused");
    pauseTitle.setCharacterSize(80);
    pauseTitle.setFillColor(Color::White);
    pauseTitle.setPosition(
        (window.getSize().x - pauseTitle.getLocalBounds().width) / 2,
        window.getSize().y / 2 - 150 // Centered vertically, offset upward
    );

    // Resume text
    resumeText.setFont(font);
    resumeText.setString("Press R to Resume");
    resumeText.setCharacterSize(40);
    resumeText.setFillColor(Color::Yellow);
    resumeText.setPosition(
        (window.getSize().x - resumeText.getLocalBounds().width) / 2,
        pauseTitle.getPosition().y + 120 // Below the title
    );

    // Quit text
    quitText.setFont(font);
    quitText.setString("Press Q to Quit");
    quitText.setCharacterSize(40);
    quitText.setFillColor(Color::Yellow);
    quitText.setPosition(
        (window.getSize().x - quitText.getLocalBounds().width) / 2,
        resumeText.getPosition().y + 60 // Below the resume text
    );
}

void PauseScreen::handleEvent(Event& event, bool& resumeGame, bool& quitToMainMenu) {
    if (event.type == Event::KeyPressed) {
        if (event.key.code == Keyboard::R) {
            resumeGame = true;
        }
        if (event.key.code == Keyboard::Q) {
            quitToMainMenu = true;
        }
    }
}

void PauseScreen::render(RenderWindow& window) {
    window.draw(overlay);
    window.draw(pauseTitle);
    window.draw(resumeText);
    window.draw(quitText);
}
