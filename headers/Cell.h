#include <SFML/Graphics.hpp>
#include "Position2D.h"
#include "Enums.h"

#ifndef CELL_H
#define CELL_H

class Cell {
private:

    CellType type;
    sf::Sprite sprite;
    Position2D position;
    sf::Texture texture;
    sf::Sprite temp;


public:

    Cell();

    Cell(CellType type);

    Position2D GetPosition();

    void SetPosition(int x, int y);

    sf::Sprite GetSprite();
    
    void ChangeCell(sf::Sprite sprite, CellType type);

    void SetSprite(sf::Sprite sprite);

    void SetType(CellType type);

    CellType GetType();

};

#endif