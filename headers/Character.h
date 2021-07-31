#include <SFML/Graphics.hpp>
#include "Position2D.h"
#include "Enums.h"
#include "Collider.h"

#ifndef CHARACTER_H
#define CHARACTER_H

// Класс персонажа
class Character {
private:

    sf::Sprite sprite;
    sf::Sprite collider;
    sf::IntRect intrect;
    sf::Texture TempTexture;
    sf::Sprite TempSprite;
    MoveDirections side = MoveDirections::RIGHT;
    Position2D position;
    int health = 100;
    int CHARACTER_SPEED;


public:

    Character();

    Character(sf::Sprite spritearg, Position2D positionarg, int speedarg);

    sf::Sprite GetSprite();

    sf::Sprite* GetSpriteRef();

    sf::Sprite* GetColliderRef();

    sf::Sprite GetColliderSprite();

    void Move(MoveDirections direction, float deltaTime);

    Collider GetCollider();

};

#endif