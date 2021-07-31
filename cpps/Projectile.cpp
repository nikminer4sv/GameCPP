#include "../headers/Projectile.h"
#include <cmath>

Projectile::Projectile() {}

Projectile::Projectile(sf::Sprite sprite, Position2D StartPosition, Position2D MousePosition, int speed) {

    this->sprite = sprite;
    this->StartPosition = StartPosition;
    this->MousePosition = MousePosition;
    this->speed = speed;
    sprite.setPosition(StartPosition.GetXF(), StartPosition.GetYF());
    sin = abs(StartPosition.GetYF() - MousePosition.GetYF()) / abs(StartPosition.GetXF() - MousePosition.GetXF());
    cos = abs(StartPosition.GetXF() - MousePosition.GetXF()) / abs(StartPosition.GetYF() - MousePosition.GetYF());

}

Projectile::Projectile(Position2D StartPosition, Position2D MousePosition, int speed) {

    arrow.setRadius(1.5);
    this->StartPosition = StartPosition;
    this->MousePosition = MousePosition;
    this->speed = speed;
    this->arrow.setPosition(StartPosition.GetXF(), StartPosition.GetYF());
    this->CurrentPosition = StartPosition;
    float path = std::sqrt(std::pow(StartPosition.GetYF() - MousePosition.GetYF(), 2) + pow(StartPosition.GetXF() - MousePosition.GetXF(), 2));
    sin = (StartPosition.GetYF() - MousePosition.GetYF()) / path;
    cos = (StartPosition.GetXF() - MousePosition.GetXF()) / path;

}

void Projectile::Update(float deltaTime) {

    CurrentPosition.SetXF(CurrentPosition.GetXF() - speed * deltaTime * cos);
    CurrentPosition.SetYF(CurrentPosition.GetYF() - speed * deltaTime * sin);

    arrow.setPosition(CurrentPosition.GetXF(), CurrentPosition.GetYF());

}

sf::CircleShape Projectile::GetSprite() {
    return arrow;
}