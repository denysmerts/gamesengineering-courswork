#include "AssetManager.h"
#include <iostream>

using namespace sf;
using namespace std;

AssetManager& AssetManager::getInstance() {
    static AssetManager instance; // Guaranteed to be destroyed and instantiated on first use
    return instance;
}

Texture& AssetManager::getTexture(const string& filename) {
    auto it = textures.find(filename);
    if (it == textures.end()) {
        // Load texture if not already loaded
        Texture texture;
        if (!texture.loadFromFile(filename)) {
            cerr << "Error loading texture: " << filename << endl;
            throw runtime_error("Failed to load texture");
        }
        textures[filename] = move(texture);
    }
    return textures[filename];
}

Font& AssetManager::getFont(const string& filename) {
    auto it = fonts.find(filename);
    if (it == fonts.end()) {
        // Load font if not already loaded
        Font font;
        if (!font.loadFromFile(filename)) {
            cerr << "Error loading font: " << filename << endl;
            throw runtime_error("Failed to load font");
        }
        fonts[filename] = move(font);
    }
    return fonts[filename];
}

SoundBuffer& AssetManager::getSoundBuffer(const string& filename) {
    auto it = soundBuffers.find(filename);
    if (it == soundBuffers.end()) {
        // Load sound buffer if not already loaded
        SoundBuffer buffer;
        if (!buffer.loadFromFile(filename)) {
            cerr << "Error loading sound buffer: " << filename << endl;
            throw runtime_error("Failed to load sound buffer");
        }
        soundBuffers[filename] = move(buffer);
    }
    return soundBuffers[filename];
}
