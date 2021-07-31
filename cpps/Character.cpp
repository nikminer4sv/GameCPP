#include "../headers/Character.h"

Character::Character() {}

Character::Character(sf::Sprite spritearg, Position2D positionarg, int speedarg):position(positionarg), sprite(spritearg), CHARACTER_SPEED(speedarg) {
    sprite.setOrigin(sprite.getGlobalBounds().width / 4, sprite.getGlobalBounds().height / 4);
    sprite.setPosition(position.GetXF(), position.GetYF());
    TempTexture.create(32,32);
    TempSprite.setTexture(TempTexture);
    intrect = sf::IntRect(0,64,32,32);
    collider = TempSprite;
    collider.setScale(1, 0.3);
    collider.setOrigin(sprite.getOrigin());
    collider.setPosition(sprite.getPosition().x, sprite.getPosition().y + sprite.getGlobalBounds().height / 2 - collider.getGlobalBounds().height / 2);
}

sf::Sprite Character::GetSprite() {
    return sprite;
}

sf::Sprite* Character::GetSpriteRef() {
    return &sprite;
}

sf::Sprite* Character::GetColliderRef() {
    return &collider;
}

sf::Sprite Character::GetColliderSprite() {
    return collider;
}

void Character::Move(MoveDirections direction, float deltaTime) {

    if (direction == RIGHT) {
        sprite.move(deltaTime*CHARACTER_SPEED, 0);
        if (side == LEFT) {
            intrect.width *= -1;
            intrect.left -= 32;
            sprite.setTextureRect(intrect);
        }
        side = RIGHT;
    } else if (direction == BOTTOM) {
        sprite.move(0, + deltaTime*CHARACTER_SPEED);
    } else if (direction == LEFT) {
        sprite.setTextureRect(sf::IntRect(32,64,-32,32));
        sprite.move(- deltaTime*CHARACTER_SPEED, 0);
        if (side == RIGHT) {
            intrect.width *= -1;
            intrect.left += 32;
            sprite.setTextureRect(intrect);
        }
        side = LEFT;
    } else if (direction == TOP) {
        sprite.move(0, - deltaTime*CHARACTER_SPEED);
    }

}

Collider Character::GetCollider() {
    collider.setPosition(sprite.getPosition().x, sprite.getPosition().y + sprite.getGlobalBounds().height / 2 - collider.getGlobalBounds().height / 2);
    return Collider(sprite, collider);
}