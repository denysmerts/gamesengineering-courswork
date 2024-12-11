#ifndef STORYLINEMANAGER_H
#define STORYLINEMANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class StorylineManager {
public:
    StorylineManager(sf::Font& font, sf::Vector2u windowSize);

    void handleInput(sf::Event event);  // Handle user input (Enter key)
    void update();                      // Update storyline logic
    void render(sf::RenderWindow& window); // Render storyline UI

    bool isStorylineActive() const;     // Check if storyline is still active

private:
    std::vector<std::string> storylineTexts; // List of storyline messages
    size_t currentTextIndex;                 // Index of the current message
    bool storylineActive;                    // Whether the storyline is active

    sf::RectangleShape textBox;              // Text box for storyline
    sf::Text storylineText;                  // Text for storyline
};

#endif // STORYLINEMANAGER_H
