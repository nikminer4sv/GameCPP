#include <SFML/Graphics.hpp>
#include "Position2D.h"

#ifndef PROJECTILE_H
#define PROJECTILE_H

class Projectile {
private:

    sf::Sprite sprite;
    sf::CircleShape arrow;
    Position2D StartPosition;
    Position2D CurrentPosition;
    Position2D MousePosition;
    int speed;
    float sin;
    float cos;


public:

    Projectile();

    Projectile(sf::Sprite sprite, Position2D StartPosition, Position2D MousePosition, int speed);

    Projectile(Position2D StartPosition, Position2D MousePosition, int speed);

    void Update(float deltaTime);

    sf::CircleShape GetSprite();

};

#endif