#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace sf;

Texture GrassTileset;
Sprite GrassSprite;

const int WINDOW_WIDTH = 512;
const int WINDOW_HEIGHT = 512;
const int TEXTURE_SIZE = 32;
const string WINDOW_TITLE = "TITLE";
const string TEXTURES_PATH = "source/textures/";


enum CellType {
    GRASS
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
        sprite = GrassSprite;
        sprite.setPosition(0,0);
    }

    Cell(CellType type) {
        
        this->type = type;

        if (type == GRASS) {
            sprite = GrassSprite;
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

void LoadSource() {

    GrassTileset.loadFromFile(TEXTURES_PATH + "Grass-tileset.png");
    GrassSprite.setTexture(GrassTileset);
    GrassSprite.setTextureRect(IntRect(0,192,32,32));

}


int main() {

    LoadSource();
    vector<vector<Cell>> GameField = CreateGameField();

    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);

    while(window.isOpen()) {

        Event event;

        while(window.pollEvent(event)) {

            if(event.type == Event::Closed) {
                
                window.close();

            }

        }

        window.clear(Color::White);

        for (int i = 0; i < WINDOW_HEIGHT / TEXTURE_SIZE; i++) {
            for (int j = 0; j < WINDOW_WIDTH / TEXTURE_SIZE; j++) {
                window.draw(GameField[j][i].GetSprite());
            }
        }

        window.display();

    }

}
