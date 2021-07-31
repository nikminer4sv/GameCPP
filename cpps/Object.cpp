#include "../headers/Object.h"

Object::Object() {
    isFillerVar = true;
}

Object::Object(sf::Sprite sprite, sf::Vector2f position) {
    this->sprite = sprite;
    this->sprite.setPosition(position);
    this->sprite.setOrigin(this->sprite.getGlobalBounds().width / 2, this->sprite.getGlobalBounds().height / 2);
}

Object::Object(sf::Sprite sprite, int HowMuchItGives, int Cost, Resources ResourceType) {
    this->sprite = sprite;
    this->sprite.setOrigin(this->sprite.getGlobalBounds().width / 2, this->sprite.getGlobalBounds().height / 2);
    this->HowMuchItGives = HowMuchItGives;
    this->Cost = Cost;
    this->ResourceTypeForBuild = ResourceType;
}

Object::Object(sf::Sprite sprite) {
    this->sprite = sprite;
    this->sprite.setOrigin(this->sprite.getGlobalBounds().width / 2, this->sprite.getGlobalBounds().height / 2);
}

void Object::SetPosition(sf::Vector2f position) {
    this->sprite.setPosition(position);
}

Collider Object::GetCollider() {
    return Collider(sprite);
}

sf::Sprite Object::GetSprite() {
    return sprite;
}

sf::Sprite* Object::GetSpriteRef() {
    return &sprite;
}

bool Object::isFiller() {
    return isFillerVar;
}

bool Object::isColliding() {
    return isCollidingVar;
}

Resources Object::GetResourceTypeForBuild() {
    return ResourceTypeForBuild;
}

int Object::GetCost() {
    return Cost;
}
