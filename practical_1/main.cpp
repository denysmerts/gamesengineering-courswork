#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <memory>

using namespace sf;

class GameStateManager;
class StartScreen;
class Level1Screen;
class Player;

// Base state class
class GameState {
protected:
    GameStateManager& manager;
public:
    GameState(GameStateManager& manager) : manager(manager) {}
    virtual ~GameState() = default;
    virtual void handleEvent(const Event& event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
};

// Helper function for value clamping
template<typename T>
T clamp(const T& value, const T& min, const T& max) {
    return (value < min) ? min : ((value > max) ? max : value);
}

// Player class definition
class Player {
private:
    Sprite sprite;
    Texture texture;
    float moveSpeed;
    Vector2f velocity;
    const float ACCELERATION = 1000.0f;
    const float MAX_SPEED = 300.0f;
    const float FRICTION = 0.85f;
    IntRect textureRect;
    int currentFrame;
    int numFrames;
    float frameTimer;
    float frameDelay;
    bool isAttacking;
    int attackFrame;
    int numAttackFrames;
    float attackTimer;
    float attackDuration;

public:
    Player(const std::string& filePath)
        : moveSpeed(300.0f), velocity(0.0f, 0.0f),
        currentFrame(0), numFrames(6), frameTimer(0.0f), frameDelay(0.1f),
        isAttacking(false), attackFrame(0), numAttackFrames(6),
        attackTimer(0.0f), attackDuration(0.6f)
    {
        if (!texture.loadFromFile(filePath)) {
            throw std::runtime_error("Failed to load player texture!");
        }
        sprite.setTexture(texture);
        sprite.setTextureRect(IntRect(0, 0, 192, 192));
        sprite.setOrigin(96, 96);
        sprite.setPosition(400, 300);
    }

    void attack() {
        if (!isAttacking) {
            isAttacking = true;
            attackFrame = 0;
            attackTimer = 0.0f;
        }
    }

    void update(float deltaTime) {
        if (isAttacking) {
            updateAttackAnimation(deltaTime);
        }
        else {
            updateMovement(deltaTime);
        }
    }

    void updateMovement(float deltaTime) {
        bool isMoving = false;

        if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)) {
            velocity.x -= ACCELERATION * deltaTime;
            sprite.setScale(-1.0f, 1.0f);
            isMoving = true;
        }
        if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) {
            velocity.x += ACCELERATION * deltaTime;
            sprite.setScale(1.0f, 1.0f);
            isMoving = true;
        }
        if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) {
            velocity.y -= ACCELERATION * deltaTime;
            isMoving = true;
        }
        if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)) {
            velocity.y += ACCELERATION * deltaTime;
            isMoving = true;
        }

        velocity.x = clamp(velocity.x, -MAX_SPEED, MAX_SPEED);
        velocity.y = clamp(velocity.y, -MAX_SPEED, MAX_SPEED);
        velocity *= FRICTION;

        sprite.move(velocity * deltaTime);

        Vector2f pos = sprite.getPosition();
        pos.x = clamp(pos.x, 0.0f, 800.0f);
        pos.y = clamp(pos.y, 0.0f, 600.0f);
        sprite.setPosition(pos);

        if (isMoving) {
            frameTimer += deltaTime;
            if (frameTimer >= frameDelay) {
                frameTimer = 0.0f;
                currentFrame = (currentFrame + 1) % numFrames;
                setTextureRect(IntRect(currentFrame * 192, 0, 192, 192));
            }
        }
        else {
            setTextureRect(IntRect(0, 0, 192, 192));
        }
    }

    void updateAttackAnimation(float deltaTime) {
        attackTimer += deltaTime;
        if (attackTimer >= attackDuration / numAttackFrames) {
            attackTimer = 0.0f;
            attackFrame++;
            if (attackFrame >= numAttackFrames) {
                isAttacking = false;
                setTextureRect(IntRect(0, 0, 192, 192));
            }
            else {
                setTextureRect(IntRect(attackFrame * 192, 384, 192, 192));
            }
        }
    }

    void setTextureRect(const IntRect& rect) {
        textureRect = rect;
        sprite.setTextureRect(textureRect);
    }

    void draw(RenderWindow& window) {
        window.draw(sprite);
    }

    const Vector2f& getPosition() const {
        return sprite.getPosition();
    }
};

class GameStateManager {
private:
    std::unique_ptr<GameState> currentState;
    RenderWindow& window;
    Font gameFont;
    Texture cursorTexture;
    Sprite customCursor;
    Texture playerTexture;

public:
    GameStateManager(RenderWindow& window) : window(window) {
        loadAssets();
        window.setMouseCursorVisible(false);
    }

    void loadAssets() {
        if (!gameFont.loadFromFile("output/assets/IrishGrover-Regular.ttf")) {
            throw std::runtime_error("Failed to load font!");
        }
        if (!cursorTexture.loadFromFile("output/assets/cursor.png")) {
            throw std::runtime_error("Failed to load cursor!");
        }
        if (!playerTexture.loadFromFile("output/assets/Warrior_Blue.png")) {
            throw std::runtime_error("Failed to load player texture!");
        }
        customCursor.setTexture(cursorTexture);
    }

    void setState(std::unique_ptr<GameState> newState) {
        currentState = std::move(newState);
    }

    void handleEvent(const Event& event) {
        if (currentState) {
            currentState->handleEvent(event);
        }
    }

    void update(float deltaTime) {
        if (currentState) {
            currentState->update(deltaTime);
        }
    }

    void render() {
        if (currentState) {
            currentState->render();
            Vector2i mousePos = Mouse::getPosition(window);
            customCursor.setPosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
            window.draw(customCursor);
        }
    }

    const Font& getFont() const { return gameFont; }
    const Texture& getPlayerTexture() const { return playerTexture; }
    RenderWindow& getWindow() { return window; }
};

class Level1Screen : public GameState {
private:
    Player player;
    Sprite background;
    Text levelTitle;
    Texture backgroundTexture;

public:
    Level1Screen(GameStateManager& manager)
        : GameState(manager), player("output/assets/Warrior_Blue.png")
    {
        if (!backgroundTexture.loadFromFile("output/assets/bg.png")) {
            throw std::runtime_error("Failed to load level 1 background!");
        }
        background.setTexture(backgroundTexture);
        levelTitle.setFont(manager.getFont());
        levelTitle.setString("Level 1");
        levelTitle.setCharacterSize(30);
        levelTitle.setPosition(10, 10);
        levelTitle.setFillColor(Color::White);
    }

    void handleEvent(const Event& event) override {
        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
            player.attack();
        }
    }

    void update(float deltaTime) override {
        player.update(deltaTime);
    }

    void render() override {
        auto& window = manager.getWindow();
        window.draw(background);
        window.draw(levelTitle);
        player.draw(window);
    }
};

class StartScreen : public GameState {
private:
    Sprite background;
    Text title;
    Text startButton;
    Texture backgroundTexture;

public:
    StartScreen(GameStateManager& manager) : GameState(manager) {
        if (!backgroundTexture.loadFromFile("output/assets/bg.png")) {
            throw std::runtime_error("Failed to load background!");
        }
        background.setTexture(backgroundTexture);
        title.setFont(manager.getFont());
        title.setString("Goblin Siege");
        title.setCharacterSize(100);
        title.setPosition(120, 50);
        title.setFillColor(Color::White);
        startButton.setFont(manager.getFont());
        startButton.setString("Start Game");
        startButton.setCharacterSize(40);
        startButton.setPosition(300, 400);
        startButton.setFillColor(Color::White);
    }

    void handleEvent(const Event& event) override {
        if (event.type == Event::MouseButtonPressed) {
            Vector2i mousePos = Mouse::getPosition(manager.getWindow());
            if (startButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                manager.setState(std::make_unique<Level1Screen>(manager));
            }
        }
    }

    void update(float deltaTime) override {}

    void render() override {
        auto& window = manager.getWindow();
        window.draw(background);
        window.draw(title);
        window.draw(startButton);
    }
};

int main() {
    RenderWindow window(VideoMode(800, 600), "Goblin Siege");
    window.setFramerateLimit(60);

    try {
        GameStateManager gameManager(window);
        gameManager.setState(std::make_unique<StartScreen>(gameManager));

        Clock clock;
        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();
                gameManager.handleEvent(event);
            }

            float deltaTime = clock.restart().asSeconds();
            gameManager.update(deltaTime);

            window.clear();
            gameManager.render();
            window.display();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}