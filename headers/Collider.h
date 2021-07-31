#include <SFML/Graphics.hpp>

#ifndef COLLIDER_H
#define COLLIDER_H

class Collider {
private:

    sf::Sprite& sprite;
    sf::Sprite& collider;


public:

    Collider(sf::Sprite& body);

    Collider(sf::Sprite& body, sf::Sprite& coll);

    bool CheckCollision(Collider& OtherObject, float push, bool ExtenderCollider);

    sf::Vector2f GetPosition();

    sf::Vector2f GetExtendedColliderPosition();

    sf::Vector2f GetHalfSize();

    sf::Vector2f GetExtendedColliderHalfSize();

    void Move(float dx, float dy, bool ExtendedCollider);

};

#endif