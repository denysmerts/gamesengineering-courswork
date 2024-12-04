#include "AssetManager.h"
#include <iostream>

AssetManager& AssetManager::getInstance() {
    static AssetManager instance; // Guaranteed to be destroyed and instantiated on first use
    return instance;
}

sf::Texture& AssetManager::getTexture(const std::string& filename) {
    auto it = textures.find(filename);
    if (it == textures.end()) {
        // Load texture if not already loaded
        sf::Texture texture;
        if (!texture.loadFromFile(filename)) {
            std::cerr << "Error loading texture: " << filename << std::endl;
            throw std::runtime_error("Failed to load texture");
        }
        textures[filename] = std::move(texture);
    }
    return textures[filename];
}

sf::Font& AssetManager::getFont(const std::string& filename) {
    auto it = fonts.find(filename);
    if (it == fonts.end()) {
        // Load font if not already loaded
        sf::Font font;
        if (!font.loadFromFile(filename)) {
            std::cerr << "Error loading font: " << filename << std::endl;
            throw std::runtime_error("Failed to load font");
        }
        fonts[filename] = std::move(font);
    }
    return fonts[filename];
}

sf::SoundBuffer& AssetManager::getSoundBuffer(const std::string& filename) {
    auto it = soundBuffers.find(filename);
    if (it == soundBuffers.end()) {
        // Load sound buffer if not already loaded
        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile(filename)) {
            std::cerr << "Error loading sound buffer: " << filename << std::endl;
            throw std::runtime_error("Failed to load sound buffer");
        }
        soundBuffers[filename] = std::move(buffer);
    }
    return soundBuffers[filename];
}
