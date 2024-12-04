#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>
#include <memory>

using namespace std;
using namespace sf;

class AssetManager {
public:
    // Get the singleton instance
    static AssetManager& getInstance();

    // Load and retrieve textures
    Texture& getTexture(const string& filename);

    // Load and retrieve fonts
    Font& getFont(const string& filename);

    // Load and retrieve sound buffers
    SoundBuffer& getSoundBuffer(const string& filename);

private:
    // Private constructor to prevent instantiation
    AssetManager() = default;

    // Deleted copy constructor and assignment operator
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    // Maps for caching loaded assets
    unordered_map<string, Texture> textures;
    unordered_map<string, Font> fonts;
    unordered_map<string, SoundBuffer> soundBuffers;
};
