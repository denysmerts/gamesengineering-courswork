#include "Map.h"
#include <iostream>

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

 /*   mapObjects.push_back({ "output/assets/Castle.png", Vector2f(250, 23) });
    mapObjects.push_back({ "output/assets/Tower.png", Vector2f(160, 410) });
    mapObjects.push_back({ "output/assets/Tower.png", Vector2f(670, 650) });
    mapObjects.push_back({ "output/assets/House.png", Vector2f(550, 0) });
    mapObjects.push_back({ "output/assets/House.png", Vector2f(770, 50) });
    mapObjects.push_back({ "output/assets/House.png", Vector2f(900, 10) });
    mapObjects.push_back({ "output/assets/House.png", Vector2f(900, 200) });
    mapObjects.push_back({ "output/assets/GoldMine.png", Vector2f(1000, 400) });
  */
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
    // Step 1: Render water tiles across the entire map
    for (size_t row = 0; row < MAP_HEIGHT; row++) {
        for (size_t col = 0; col < MAP_WIDTH; col++) {
            size_t index = row * MAP_WIDTH + col;

            sprites[index].setTexture(waterTexture);
            sprites[index].setPosition(Vector2f(100 + col * titleWidth, 100 + row * titleHeight));
            window.draw(sprites[index]);
        }
    }

    // Step 2: Render elevation and ground tiles
    for (size_t row = 0; row < MAP_HEIGHT; row++) {
        for (size_t col = 0; col < MAP_WIDTH; col++) {
            size_t index = row * MAP_WIDTH + col;
            int tileID = mapData[row][col];



            // Elevation tiles
            int elevationID = elevationData[row][col];
            if (elevationID > 0) {
                Sprite elevationSprite;
                elevationSprite.setTexture(elevationTexture);
                elevationSprite.setTextureRect(IntRect((elevationID % 10) * titleWidth, (elevationID / 10) * titleHeight, titleWidth, titleHeight));
                elevationSprite.setPosition(Vector2f(100 + col * titleWidth, 100 + row * titleHeight));
                window.draw(elevationSprite);
            }

            // Ground tiles
            sprites[index].setTexture(texture);
            sprites[index].setTextureRect(IntRect((tileID % 10) * titleWidth, (tileID / 10) * titleHeight, titleWidth, titleHeight));
            sprites[index].setPosition(Vector2f(100 + col * titleWidth, 100 + row * titleHeight));
            window.draw(sprites[index]);

        }
    }

    // Step 3: Render objects on top
    for (const auto& obj : mapObjects) {
        window.draw(obj.sprite);
    }
}
