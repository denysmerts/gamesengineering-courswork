#include "Map.h"
#include <iostream>
using namespace sf;

using namespace std;

Map::Map()
    : titleWidth(64), titleHeight(64) {}

Map::~Map() {}

void Map::load() {

    if (!texture.loadFromFile("output/assets/Tilemap.png")) {
        cerr << "Error loading tilemap texture!" << endl;
    }

    if (!waterTexture.loadFromFile("output/assets/Water.png")) {
        cerr << "Error loading water texture!" << endl;
    }

    if (!elevationTexture.loadFromFile("output/assets/Elevation.png")) {
    cerr << "Error loading elevation texture!" << endl;
    }

    mapObjects.push_back({ "output/assets/Castle.png", Vector2f(260, 100) });
    mapObjects.push_back({ "output/assets/Tower.png", Vector2f(225, 480) });
    mapObjects.push_back({ "output/assets/Tower.png", Vector2f(735, 725) });
    mapObjects.push_back({ "output/assets/House.png", Vector2f(940, 130) });
    mapObjects.push_back({ "output/assets/House.png", Vector2f(680, 90) });
    mapObjects.push_back({ "output/assets/House.png", Vector2f(1060, 100) });
    mapObjects.push_back({ "output/assets/House.png", Vector2f(1060, 300) });
    mapObjects.push_back({ "output/assets/GoldMine.png", Vector2f(1250, 250) });
  
    for (auto& obj : mapObjects) {
        if (!obj.texture.loadFromFile(obj.texturePath)) {
            cerr << "Error loading texture: " << obj.texturePath << endl;
        }
        obj.sprite.setTexture(obj.texture);
        obj.sprite.setPosition(obj.position);
    }

    for (size_t row = 0; row < MAP_HEIGHT; row++) {
        for (size_t col = 0; col < MAP_WIDTH; col++) {
            size_t index = row * MAP_WIDTH + col;

            if (index < 400) {
                sprites[index].setTexture(texture);

                int tileID = mapData[row][col];

                if (tileID == 14) {
                    spriteRect = IntRect(0, 0, titleWidth, titleHeight);  
                    sprites[index].setTexture(waterTexture); 
                }
                else {
                    spriteRect = IntRect((tileID % 10) * titleWidth, (tileID / 10) * titleHeight, titleWidth, titleHeight);
                    sprites[index].setTexture(texture);
                }

                sprites[index].setPosition(Vector2f(100 + col * titleWidth, 100 + row * titleHeight));
                sprites[index].setTextureRect(spriteRect);
            }
        }
    }


}

void Map::update() {
    
}

void Map::render(RenderWindow& window) {
    /* Render water as fullscreen background */
    Sprite waterBackground;
    waterBackground.setTexture(waterTexture);

    // Scale the water sprite to cover the entire screen
    Vector2u windowSize = window.getSize();
    Vector2u textureSize = waterTexture.getSize();

    float scaleX = static_cast<float>(windowSize.x) / textureSize.x + 1;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y + 1;

    waterBackground.setScale(scaleX, scaleY);
    waterBackground.setPosition(0, 0);

    // Draw the water background layer first
    window.draw(waterBackground);


    // Render water tiles
    for (size_t row = 0; row < MAP_HEIGHT; row++) {
        for (size_t col = 0; col < MAP_WIDTH; col++) {
            size_t index = row * MAP_WIDTH + col;

            sprites[index].setTexture(waterTexture);
            sprites[index].setPosition(Vector2f(100 + col * titleWidth, 100 + row * titleHeight));
            window.draw(sprites[index]);
        }
    }

     /*Render sand and first elevation tiles*/
    for (size_t row = 0; row < MAP_HEIGHT; row++) {
        for (size_t col = 0; col < MAP_WIDTH; col++) {
            size_t index = row * MAP_WIDTH + col;
            int tileID = sandData[row][col];

            // Render sand tiles
            sprites[index].setTexture(texture);
            sprites[index].setTextureRect(IntRect((tileID % 10) * titleWidth, (tileID / 10) * titleHeight, titleWidth, titleHeight));
            sprites[index].setPosition(Vector2f(100 + col * titleWidth, 100 + row * titleHeight));
            window.draw(sprites[index]);
        }
    }

     /*Render ground and first elevation tiles*/
    for (size_t row = 0; row < MAP_HEIGHT; row++) {
        for (size_t col = 0; col < MAP_WIDTH; col++) {
            size_t index = row * MAP_WIDTH + col;
            int tileID = mapData[row][col];

            // Render first elevation tiles
            int elevationID = elevationData[row][col];
            if (elevationID > 0) {
                Sprite elevationSprite;
                elevationSprite.setTexture(elevationTexture);
                elevationSprite.setTextureRect(IntRect((elevationID % 10) * titleWidth, (elevationID / 10) * titleHeight, titleWidth, titleHeight));
                elevationSprite.setPosition(Vector2f(100 + col * titleWidth, 100 + row * titleHeight));
                window.draw(elevationSprite);
            }

            // Render ground tiles
            sprites[index].setTexture(texture);
            sprites[index].setTextureRect(IntRect((tileID % 10) * titleWidth, (tileID / 10) * titleHeight, titleWidth, titleHeight));
            sprites[index].setPosition(Vector2f(100 + col * titleWidth, 100 + row * titleHeight));
            window.draw(sprites[index]);

        }
    }

     /*Render second elevation tiles*/
    for (size_t row = 0; row < MAP_HEIGHT; row++) {
        for (size_t col = 0; col < MAP_WIDTH; col++) {
            int secondElevationID = secondElevationData[row][col];
            if (secondElevationID > 0) {
                Sprite secondElevationSprite;
                secondElevationSprite.setTexture(elevationTexture); // Use appropriate texture
                secondElevationSprite.setTextureRect(IntRect((secondElevationID % 10) * titleWidth, (secondElevationID / 10) * titleHeight, titleWidth, titleHeight));
                secondElevationSprite.setPosition(Vector2f(100 + col * titleWidth, 100 + row * titleHeight));
                window.draw(secondElevationSprite);
            }
        }
    }

     /*Render second ground tiles (at the end)*/
    for (size_t row = 0; row < MAP_HEIGHT; row++) {
        for (size_t col = 0; col < MAP_WIDTH; col++) {
            int secondGroundTileID = secondGroundData[row][col];
            if (secondGroundTileID >= 0) { // Only render non-empty tiles
                Sprite secondGroundSprite;
                secondGroundSprite.setTexture(texture); // Use the same ground texture
                secondGroundSprite.setTextureRect(IntRect(
                    (secondGroundTileID % 10) * titleWidth,
                    (secondGroundTileID / 10) * titleHeight,
                    titleWidth, titleHeight
                ));
                secondGroundSprite.setPosition(Vector2f(100 + col * titleWidth, 100 + row * titleHeight));
                window.draw(secondGroundSprite);
            }
        }
    }

    // Render objects
    for (const auto& obj : mapObjects) {
        window.draw(obj.sprite);
    }
}

bool Map::isWaterTile(float x, float y) const {
    // Convert world coordinates to tile coordinates
    int col = static_cast<int>((x-100) / titleWidth);
    int row = static_cast<int>((y-100) / titleHeight);

    // Check bounds
    if (row < 0 || row >= MAP_HEIGHT || col < 0 || col >= MAP_WIDTH) {
        return false;
    }

    // Check if the tile is water (tile ID 14)
    return wallData[row][col] == 14;
}