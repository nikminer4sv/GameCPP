#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <stdlib.h>
#include <ctime>

using namespace std;
using namespace sf;

Texture CharacterTileset;
Sprite CharacterSprite;
Texture GroundTileset;
vector<Sprite> GrassSprites(32);
vector<Sprite> RoadSprites(32);

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1088;
const int VIEW_WIDTH = 850;
const int VIEW_HEIGHT = 450;
const int WORLD_WIDTH = 50;
const int TEXTURE_SIZE = 32;
const int CHARACTER_SIZE = 32;
const int CHARACTER_SCALE_FACTOR = 2;
const int CHARACTER_SPEED = 200;
const string WINDOW_TITLE = "Game";
const string TEXTURES_PATH = "source/textures/";
const bool isGenerator = true;


enum CellType {
    GRASS,
    ROAD
};

enum MoveDirections {
    TOP,
    RIGHT,
    BOTTOM,
    LEFT
};

class Position2D {
private:
    int x;
    int y;
    float xf;
    float yf;

public:

    Position2D() {
        x = 0;
        y = 0;
        xf = 0;
        yf = 0;
    }

    Position2D(int x, int y) {
        this->x = x;
        this->y = y;
    }

    Position2D(float xf, float yf) {
        this->xf = xf;
        this->yf = yf;
    }

    int GetX() {
        return x;
    }

    int GetY() {
        return y;
    }

    void SetX(int x) {
        this->x = x;
    }

    void SetY(int y) {
        this->y = y;
    }

    void SetXY(int x, int y) {
        this->x = x;
        this->y = y;
    }

    float GetXF() {
        return xf;
    }

    float GetYF() {
        return yf;
    }

    void SetXF(float xf) {
        this->xf = xf;
    }

    void SetYF(float yf) {
        this->yf = yf;
    }

    void SetXYF(float xf, float yf) {
        this->xf = xf;
        this->yf = yf;
    }

};

class Cell {
private:
    CellType type;
    Sprite sprite;
    Position2D position;

public:
    Cell() {
        type = GRASS;
        sprite = GrassSprites[0];
        sprite.setPosition(0,0);
    }

    Cell(CellType type) {
        
        this->type = type;

        if (type == GRASS) {
            sprite = GrassSprites[0];
        } else if (type == ROAD) {
            sprite = RoadSprites[0];
        }

        sprite.setPosition(0,0);
    }

    Position2D GetPosition() {
        return position;
    }

    void SetPosition(int x, int y) {
        position.SetXY(x,y);
        sprite.setPosition(position.GetX(), position.GetY());
    }

    Sprite GetSprite() {
        return sprite;
    }
    
    void ChangeCell(Sprite sprite, CellType type) {
        sprite.setPosition(position.GetX(), position.GetY());
        SetSprite(sprite);
        SetType(type);
    }

    void SetSprite(Sprite sprite) {
        this->sprite = sprite;
    }

    void SetType(CellType type) {
        this->type = type;
    }

};

void LoadSource() {

    GroundTileset.loadFromFile(TEXTURES_PATH + "GroundTileset.png");

    for (int i = 0; i < 32; i++) {
        GrassSprites[i].setTexture(GroundTileset);
        GrassSprites[i].setTextureRect(IntRect((i % 8)*TEXTURE_SIZE,(i / 8)*TEXTURE_SIZE,32,32));
    }

    for (int i = 0; i < 32; i++) {
        RoadSprites[i].setTexture(GroundTileset);
        RoadSprites[i].setTextureRect(IntRect((i % 8)*TEXTURE_SIZE,(i / 8)*TEXTURE_SIZE+128,32,32));
    }

    // Character

    CharacterTileset.loadFromFile(TEXTURES_PATH + "Character.png");

    CharacterSprite.setTexture(CharacterTileset);
    CharacterSprite.setPosition(WINDOW_WIDTH/2 - CHARACTER_SIZE/2, WINDOW_HEIGHT/2 - CHARACTER_SIZE/2);
    CharacterSprite.setTextureRect(IntRect(0,64,32,32));
    CharacterSprite.scale(CHARACTER_SCALE_FACTOR, CHARACTER_SCALE_FACTOR);

}

class Character {
private:
    Sprite sprite;
    Position2D position;
    int health = 100;
    IntRect intrect;
    MoveDirections side = RIGHT;

public:
    Character() {}

    Character(Sprite spritearg, Position2D positionarg):position(positionarg), sprite(spritearg) {
        sprite.setPosition(position.GetXF(), position.GetYF());
        intrect = IntRect(0,64,32,32);
    }

    Sprite GetSprite() {
        return sprite;
    }

    Sprite* GetSpriteRef() {
        return &sprite;
    }

    Position2D GetPosition() {
        return position;
    }

    void Move(MoveDirections direction, float deltaTime) {

        if (direction == RIGHT) {
            position.SetXF(position.GetXF() + deltaTime*CHARACTER_SPEED);
            sprite.setPosition(position.GetXF(), position.GetYF());
            if (side == LEFT) {
                intrect.width *= -1;
                intrect.left -= 32;
                sprite.setTextureRect(intrect);
            }
            side = RIGHT;
        } else if (direction == BOTTOM) {
            position.SetYF(position.GetYF() + deltaTime*CHARACTER_SPEED);
            sprite.setPosition(position.GetXF(), position.GetYF());
        } else if (direction == LEFT) {
            sprite.setTextureRect(IntRect(32,64,-32,32));
            position.SetXF(position.GetXF() - deltaTime*CHARACTER_SPEED);
            sprite.setPosition(position.GetXF(), position.GetYF());
            if (side == RIGHT) {
                intrect.width *= -1;
                intrect.left += 32;
                sprite.setTextureRect(intrect);
            }
            side = LEFT;
        } else if (direction == TOP) {
            position.SetYF(position.GetYF() - deltaTime*CHARACTER_SPEED);
            sprite.setPosition(position.GetXF(), position.GetYF());
        }

    }

};

class Animation {
private:
    float SwitchTime;
    Sprite sprite;
    Vector2u ImageCount;
    Vector2u CurrentImage;
    IntRect UvRect;
    float TotalTime;

public:
    Animation() {}

    Animation(Sprite sprite, float SwitchTime, Vector2u ImageCount) {
        this->sprite = sprite;
        this->SwitchTime = SwitchTime;
        this->ImageCount = ImageCount;
        TotalTime = 0.0f;
        CurrentImage.x = 0;

        UvRect.width = this->sprite.getTexture()->getSize().x / float(ImageCount.x);
        UvRect.height = this->sprite.getTexture()->getSize().y / float(ImageCount.y);

    }

    void Update(int Row, float deltaTime, bool FaceRight) {
        
        CurrentImage.y = Row;
        TotalTime += deltaTime;

        if (TotalTime >= SwitchTime) {

            TotalTime -= SwitchTime;
            CurrentImage.x++;

            if (CurrentImage.x >= ImageCount.x) {
                CurrentImage.x = 0;
            }

        } 

        if (FaceRight) {
            UvRect.left = CurrentImage.x * abs(UvRect.width);
            UvRect.width = abs(UvRect.width);
        } else {
            UvRect.left = CurrentImage.x * abs(UvRect.width) + abs(UvRect.width);
            UvRect.width = -abs(UvRect.width);
        }
        UvRect.top = CurrentImage.y * UvRect.height;

    }

    IntRect GetIntRect() {
        return UvRect;
    }

};

int GetRandomNumber(int min, int max) {

    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0); 
    return static_cast<int>(rand() * fraction * (max - min + 1) + min);

}

vector<vector<Cell>> DefaultMap() {

    vector<vector<Cell>> GameField(WINDOW_HEIGHT / TEXTURE_SIZE, vector<Cell>(WINDOW_WIDTH / TEXTURE_SIZE, Cell(GRASS)));

    for (int i = 0; i < WINDOW_HEIGHT / TEXTURE_SIZE; i++) {
        for (int j = 0; j < WINDOW_WIDTH / TEXTURE_SIZE; j++) {
            int rnumber = GetRandomNumber(0,31);
            GameField[i][j].ChangeCell(GrassSprites[rnumber], GRASS);
            GameField[i][j].SetPosition(j * TEXTURE_SIZE, i * TEXTURE_SIZE);
        }
    }

    return GameField;

}

int main() {

    srand(static_cast<unsigned int>(time(0)));
    LoadSource();
    vector<vector<Cell>> GameField = DefaultMap();
    Character character(CharacterSprite, Position2D(static_cast<float>(WINDOW_WIDTH/2 - 32), static_cast<float>(WINDOW_HEIGHT/2 - 32)));
    Animation CharacterAnimation(character.GetSprite(), 0.07f, Vector2u(10,10));
    View CharacterView(FloatRect(560.f, 315.f, VIEW_WIDTH, VIEW_HEIGHT));

    RenderWindow window(VideoMode(1920, 1080), WINDOW_TITLE, Style::Fullscreen);
    window.setVerticalSyncEnabled(true);
    window.setView(CharacterView);

    int current_road_type = 0;  
    float deltaTime = 0.0f;
    Clock clock;

    bool LastFaceRight = true;

    while(window.isOpen()) {

        deltaTime = clock.restart().asSeconds();
        bool isMoving = false;

        Event event;

        while(window.pollEvent(event)) {

            if (event.type == Event::Closed) {
                
                window.close();

            }

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Right) {
                if (current_road_type + 1 == 32) {
                    current_road_type = 0;
                } else {
                    current_road_type++;
                }
            }

        }

        bool updated = false;

        if (Keyboard::isKeyPressed(Keyboard::D)) {
            if (character.GetPosition().GetXF() + deltaTime*CHARACTER_SPEED + CHARACTER_SIZE * CHARACTER_SCALE_FACTOR - 1 <= WINDOW_WIDTH) {
                character.Move(RIGHT, deltaTime);
                if (updated == false) { 
                    CharacterAnimation.Update(2, deltaTime, true); 
                    updated = true;
                }
                if (CharacterView.getCenter().x + VIEW_WIDTH/2 + deltaTime*CHARACTER_SPEED <= WINDOW_WIDTH) {
                    if (!(character.GetPosition().GetXF() + CHARACTER_SIZE*CHARACTER_SCALE_FACTOR/2 < CharacterView.getCenter().x)) {
                        CharacterView.move(deltaTime*CHARACTER_SPEED, 0.f);
                        window.setView(CharacterView);
                    }
                }
                LastFaceRight = true;
                isMoving = true;
            }
        } 

        if (Keyboard::isKeyPressed(Keyboard::S)) {
            if (character.GetPosition().GetYF() + deltaTime*CHARACTER_SPEED + CHARACTER_SIZE * CHARACTER_SCALE_FACTOR - 1 <= WINDOW_HEIGHT) {
                character.Move(BOTTOM, deltaTime);
                if (updated == false) {
                    CharacterAnimation.Update(2, deltaTime, LastFaceRight);
                    updated = true;
                }
                if (CharacterView.getCenter().y + VIEW_HEIGHT/2 + deltaTime*CHARACTER_SPEED <= WINDOW_HEIGHT) {
                    if (!(character.GetPosition().GetYF() + CHARACTER_SIZE*CHARACTER_SCALE_FACTOR/2 < CharacterView.getCenter().y)) {
                        CharacterView.move(0.f, deltaTime*CHARACTER_SPEED);
                        window.setView(CharacterView);
                    }
                }
                isMoving = true;
            }
        } 

        if (Keyboard::isKeyPressed(Keyboard::A)) {
            if (character.GetPosition().GetXF() - deltaTime*CHARACTER_SPEED + 1 >= 0) {
                character.Move(LEFT, deltaTime);
                if (updated == false) {
                    CharacterAnimation.Update(2, deltaTime, false);
                    updated = true;
                }
                if (CharacterView.getCenter().x - VIEW_WIDTH/2 - deltaTime*CHARACTER_SPEED >= 0) {

                    if (!(character.GetPosition().GetXF() + CHARACTER_SIZE*CHARACTER_SCALE_FACTOR/2 > CharacterView.getCenter().x)) {
                        CharacterView.move(-deltaTime*CHARACTER_SPEED, 0.f);
                        window.setView(CharacterView);
                    }

                }
                LastFaceRight = false;
                isMoving = true;
            }
        } 

        if (Keyboard::isKeyPressed(Keyboard::W)) {
            if (character.GetPosition().GetYF() - deltaTime*CHARACTER_SPEED >= 0) {
                character.Move(TOP, deltaTime);
                if (updated == false) {
                    CharacterAnimation.Update(2, deltaTime, LastFaceRight);
                    updated = true;
                }
                if (CharacterView.getCenter().y - VIEW_HEIGHT/2 - deltaTime*CHARACTER_SPEED >= 0) {
                    if (!(character.GetPosition().GetYF() + CHARACTER_SIZE*CHARACTER_SCALE_FACTOR/2 > CharacterView.getCenter().y)) {
                        CharacterView.move(0.f, -deltaTime*CHARACTER_SPEED);
                        window.setView(CharacterView);
                    }
                }
                isMoving = true;
            }
        }

        if (isGenerator) {
            if (Mouse::isButtonPressed(Mouse::Left)) {
                int col, row;
                col = window.mapPixelToCoords(Mouse::getPosition(window)).x / TEXTURE_SIZE;
                row = window.mapPixelToCoords(Mouse::getPosition(window)).y / TEXTURE_SIZE;
                GameField[row][col].ChangeCell(RoadSprites[current_road_type], ROAD);         
            }
        }

        for (int i = 0; i < WINDOW_HEIGHT / TEXTURE_SIZE; i++) {
            for (int j = 0; j < WINDOW_WIDTH / TEXTURE_SIZE; j++) {
                window.draw(GameField[i][j].GetSprite());
            }
        }

        if (isMoving == false) {
            CharacterAnimation.Update(0, deltaTime, LastFaceRight);
        }

        character.GetSpriteRef()->setTextureRect(CharacterAnimation.GetIntRect());
        window.draw(character.GetSprite());

        window.display();

    }

}
