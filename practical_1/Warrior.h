#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

int main() {
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Goblin Siege - Start Screen");
    window.setFramerateLimit(60);

    // Load the background texture
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/bg.webp")) {
        std::cerr << "Failed to load background image!" << std::endl;
        return -1;
    }
    sf::Sprite background(backgroundTexture);

    // Load the font for the text
    sf::Font font;
    if (!font.loadFromFile("path/to/your/font.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return -1;
    }

    // Create the title text
    sf::Text title("Goblin Siege", font, 50);
    title.setPosition(200, 50);  // Adjust position as needed
    title.setFillColor(sf::Color::White);

    // Create the "Start" button
    sf::Text startButton("Start Game", font, 40);
    startButton.setPosition(300, 400);  // Adjust position as needed
    startButton.setFillColor(sf::Color::White);

    // Main game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Handle mouse click on the "Start" button
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (startButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    std::cout << "Starting the game..." << std::endl;
                    // Add your logic to transition to the game
                }
            }
        }

        // Render the start screen
        window.clear();
        window.draw(background);
        window.draw(title);
        window.draw(startButton);
        window.display();
    }

    return 0;
}

