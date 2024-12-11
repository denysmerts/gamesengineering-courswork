#include "StorylineManager.h"

StorylineManager::StorylineManager(sf::Font& font, sf::Vector2u windowSize)
    : currentTextIndex(0), storylineActive(true) {
    // Initialize storyline texts
    storylineTexts = {
        "Villager: Oh no! Our village is under attack! \nPRESS 'ENTER'",
        "Villager: We need a brave warrior to save us!\nPRESS 'ENTER'"
    };

    // Initialize the text box
    textBox.setSize(sf::Vector2f(windowSize.x * 0.8f, 150));
    textBox.setFillColor(sf::Color(0, 0, 0, 200));
    textBox.setPosition((windowSize.x - textBox.getSize().x) / 2,
        windowSize.y - textBox.getSize().y - 50);

    // Initialize the text
    storylineText.setFont(font);
    storylineText.setCharacterSize(30);
    storylineText.setFillColor(sf::Color::White);
    storylineText.setPosition(textBox.getPosition().x + 20, textBox.getPosition().y + 20);
    storylineText.setString(storylineTexts[currentTextIndex]);
}

void StorylineManager::handleInput(sf::Event event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
        currentTextIndex++;
        if (currentTextIndex >= storylineTexts.size()) {
            storylineActive = false;
        }
        else {
            storylineText.setString(storylineTexts[currentTextIndex]);
        }
    }
}

void StorylineManager::update() {
    // No real update logic here for now
}

void StorylineManager::render(sf::RenderWindow& window) {
    if (storylineActive) {
        window.draw(textBox);
        window.draw(storylineText);
    }
}

bool StorylineManager::isStorylineActive() const {
    return storylineActive;
}
