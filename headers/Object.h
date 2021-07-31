#include <SFML/Graphics.hpp>
#include "Enums.h"
#include "Collider.h"

#ifndef OBJECT_H
#define OBJECT_H

class Object {
private:

    sf::Sprite sprite;
    bool isObjectGivesSomething = false;
    bool isFillerVar = false;
    bool isCollidingVar = true;
    int Cost = 0;
    int HowMuchItGives = 0;
    Resources ResourceTypeForBuild;


public:

    Object();

    Object(sf::Sprite sprite, sf::Vector2f position);

    Object(sf::Sprite sprite, int HowMuchItGives, int Cost, Resources ResourceType);

    Object(sf::Sprite sprite);

    void SetPosition(sf::Vector2f position);

    Collider GetCollider();

    sf::Sprite GetSprite();

    sf::Sprite* GetSpriteRef();

    bool isFiller();

    bool isColliding();

    Resources GetResourceTypeForBuild();

    int GetCost();

};

#endif