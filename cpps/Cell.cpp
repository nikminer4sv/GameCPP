#include "../headers/Cell.h"

Cell::Cell() {

    //type = GRASS;
    //sprite = GrassSprites[0];
    //sprite.setPosition(0,0);

}

Cell::Cell(CellType type) {
        
    this->type = type;

    texture.create(32,32);
    temp.setTexture(texture);

    if (type == GRASS) {
        sprite = temp;
    } else if (type == ROAD) {
        sprite = temp;
    }

    sprite.setPosition(0,0);
}

Position2D Cell::GetPosition() {
    return position;
}

void Cell::SetPosition(int x, int y) {
    position.SetXY(x,y);
    sprite.setPosition(position.GetX(), position.GetY());
}

sf::Sprite Cell::GetSprite() {
    return sprite;
}
    
void Cell::ChangeCell(sf::Sprite sprite, CellType type) {
    sprite.setPosition(position.GetX(), position.GetY());
    SetSprite(sprite);
    SetType(type);
}

void Cell::SetSprite(sf::Sprite sprite) {
    this->sprite = sprite;
}

void Cell::SetType(CellType type) {
    this->type = type;
}

CellType Cell::GetType() {
    return type;
}