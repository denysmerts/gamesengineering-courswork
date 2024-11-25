#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
using namespace std;

using namespace sf;

struct MapObject {
    Texture texture;
    Sprite sprite;
    string texturePath;
    Vector2f position;

    // Constructor
    MapObject(const std::string& path, const Vector2f& pos)
        : texturePath(path), position(pos) {}
};

class Map {
public:
    Map();  
    virtual ~Map();  

    virtual void load();  
    virtual void update();  
    virtual void render(RenderWindow& window);  

    Texture texture;  
    Sprite sprite;  
    IntRect spriteRect;  

    vector<MapObject> mapObjects;

    Texture castleTexture;
    Sprite castleSprite;

    Texture towerTexture;
    Sprite towerSprite;

    Texture houseTexture;
    Sprite houseSprite;

    Texture treeTexture;
    Sprite treeSprite;

    Texture waterTexture;
    Sprite waterSprite;


    int titleWidth;
    int titleHeight;

   
  

    const static int MAP_WIDTH = 21; 
    const static int MAP_HEIGHT = 14; 
    const static int spriteSize = 294;
    Sprite sprites[spriteSize];


    int mapData[MAP_HEIGHT][MAP_WIDTH] = {
        {14, 14, 14, 14, 14, 14, 14, 14, 14,14,14,14,14,14,14,14,14,14,14,14,14},
        {14,0, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1,2,14},
        {5,10, 11, 11, 11, 11, 11, 11, 11, 11, 11,11,11,11,11,11,11,11,11,12,14},
        {15,20, 11, 11, 11, 11, 11, 11, 11, 11, 11,11,11,11,11,11,11,11,11,12,14},
        {26,14, 20, 21, 21, 21, 11, 11, 11, 11, 11,11,11,11,11,11,11,11,11,12,14},
        {14,15, 14, 14, 14, 14, 10, 11, 11, 11, 11,11,11,11,11,11,11,11,11,22,14},
        {5,14, 14, 14, 14, 14, 10, 11, 11, 11, 11,11,11,11,11,11,11,11,12,14,14},
        {15,0, 2, 14, 14, 0, 21, 21, 21, 11, 11,11,11,11,11,11,11,11,12,14,14},
        {15,20, 22, 14, 14, 23, 14, 14, 14, 20, 21,21,11,11,11,11,11,11,11,2,14},
        {15,14, 14, 14, 14, 14, 14, 14, 17, 14, 14,14,10,11,11,11,11,11,11,12,14},
        {26,26, 26, 26, 14, 14, 14, 14, 27, 14, 14,14,10,11,11,11,11,11,21,22,14},
        {14,14, 14, 14, 35, 26, 26, 37, 14, 0,2,14,20,21,21,21,21,22,14,14,14},
        {14,14, 14, 14, 14, 14, 14, 14, 14, 20,22,14,14,14,14,14,14,14,14,14,14},
        {14,14, 14, 14, 14, 14, 14, 14, 14, 14,14,14,14,14,14,14,14,14,14,14,14},
    };



    Texture elevationTexture;
    Sprite elevationSprite;

    int elevationData[MAP_HEIGHT][MAP_WIDTH] = {
        {1, 2, 3, 22, 23, 0, 0, 0, 0, 10, 11, 12, 13,0, 0, 30, 31, 32, 33, 20, 21},
        {0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 20, 41, 41, 41, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 30, 20, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 30, 31, 31, 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 22, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0},
        {0, 40, 42, 0, 0, 40, 41, 41, 41, 41, 41, 41, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 20, 22, 0, 0, 13, 70, 71, 72, 20, 41, 41, 41, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 30, 32, 0, 0, 33, 0, 0, 0, 30, 31, 31, 20, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 41, 41, 41, 41, 41, 41, 41, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 42, 0, 20, 41, 41, 41, 41, 22, 70, 72, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 22, 0, 30, 31, 31, 31, 31, 32, 0, 0,0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
 
};
