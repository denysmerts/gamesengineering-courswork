#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>
#include <memory>

class AssetManager {
public:
    // Get the singleton instance
    static AssetManager& getInstance();

    // Load and retrieve textures
    sf::Texture& getTexture(const std::string& filename);

    // Load and retrieve fonts
    sf::Font& getFont(const std::string& filename);

    // Load and retrieve sound buffers
    sf::SoundBuffer& getSoundBuffer(const std::string& filename);

private:
    // Private constructor to prevent instantiation
    AssetManager() = default;

    // Deleted copy constructor and assignment operator
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    // Maps for caching loaded assets
    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, sf::Font> fonts;
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
};
