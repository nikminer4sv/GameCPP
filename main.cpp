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
Sprite RoadSprite;
vector<Sprite> GrassSprites(32);

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 512;
const int TEXTURE_SIZE = 32;
const string WINDOW_TITLE = "TITLE";
const string TEXTURES_PATH = "source/textures/";
const bool isGenerator = true;


enum CellType {
    GRASS,
    ROAD
};

class Position2D {
private:
    int x;
    int y;

public:

    Position2D() {
        x = 0;
        y = 0;
    }

    Position2D(int x, int y) {
        this->x = x;
        this->y = y;
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
            sprite = RoadSprite;
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

vector<vector<Cell>> CreateGameField() {

    vector<vector<Cell>> GameField(WINDOW_HEIGHT / TEXTURE_SIZE, vector<Cell>(WINDOW_WIDTH / TEXTURE_SIZE, Cell(GRASS)));

    for (int i = 0; i < WINDOW_HEIGHT / TEXTURE_SIZE; i++) {
        for (int j = 0; j < WINDOW_WIDTH / TEXTURE_SIZE; j++) {
            GameField[i][j].SetPosition(j * TEXTURE_SIZE, i * TEXTURE_SIZE);
        }
    }

    return GameField;

}

void LoadSource(vector<Sprite>& GrassSprites) {

    GroundTileset.loadFromFile(TEXTURES_PATH + "GroundTileset.png");

    for (int i = 0; i < 32; i++) {
        GrassSprites[i].setTexture(GroundTileset);
        GrassSprites[i].setTextureRect(IntRect((i % 8)*TEXTURE_SIZE,(i / 8)*TEXTURE_SIZE,32,32));
    }

    RoadSprite.setTexture(GroundTileset);
    RoadSprite.setTextureRect(IntRect(0,128,32,32));

    // Character

    CharacterTileset.loadFromFile(TEXTURES_PATH + "Character.png");

    CharacterSprite.setTexture(CharacterTileset);
    CharacterSprite.setTextureRect(IntRect(0,0,32,32));
    CharacterSprite.scale(2,2);

}

class Character {
private:
    Sprite sprite;
    Position2D position;
    int health = 100;

public:
    Character() {}

    Character(Sprite spritearg, Position2D positionarg):position(positionarg), sprite(spritearg) {
        sprite.setPosition(position.GetX(), position.GetY());
    }

    Sprite GetSprite() {
        return sprite;
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
    LoadSource(GrassSprites);
    vector<vector<Cell>> GameField = DefaultMap();
    Character character(CharacterSprite, Position2D(WINDOW_WIDTH/2 - 32, WINDOW_HEIGHT/2 - 32));

    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);

    while(window.isOpen()) {

        Event event;

        while(window.pollEvent(event)) {

            if(event.type == Event::Closed) {
                
                window.close();

            }

        }

        if (isGenerator) {
            if (Mouse::isButtonPressed(Mouse::Left)) {
                if (Mouse::getPosition(window).x >= 0 && 
                    Mouse::getPosition(window).x <= WINDOW_WIDTH && 
                    Mouse::getPosition(window).y >= 0 && 
                    Mouse::getPosition(window).y <= WINDOW_HEIGHT) {
                    int col, row;
                    col = Mouse::getPosition(window).x / TEXTURE_SIZE;
                    row = Mouse::getPosition(window).y / TEXTURE_SIZE;
                    GameField[row][col].ChangeCell(RoadSprite, ROAD);         
                }
            }
        }

        for (int i = 0; i < WINDOW_HEIGHT / TEXTURE_SIZE; i++) {
            for (int j = 0; j < WINDOW_WIDTH / TEXTURE_SIZE; j++) {
                window.draw(GameField[i][j].GetSprite());
            }
        }

        window.draw(character.GetSprite());

        window.display();

    }

}
