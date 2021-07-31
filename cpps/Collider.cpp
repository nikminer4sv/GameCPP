#include "../headers/Collider.h"


Collider::Collider(sf::Sprite& body): sprite(body), collider(body) {}

Collider::Collider(sf::Sprite& body, sf::Sprite& coll): sprite(body), collider(coll) {}

bool Collider::CheckCollision(Collider& OtherObject, float push, bool ExtenderCollider) {

    if (ExtenderCollider) {

        sf::Vector2f OtherPosition = OtherObject.GetExtendedColliderPosition();
        sf::Vector2f OtherHalfSize = OtherObject.GetExtendedColliderHalfSize();
        sf::Vector2f ThisPosition = GetPosition();
        sf::Vector2f ThisHalfSize = GetHalfSize();

        float DeltaX = OtherPosition.x - ThisPosition.x;
        float DeltaY = OtherPosition.y - ThisPosition.y;

        float IntersectX = abs(DeltaX) - (OtherHalfSize.x + ThisHalfSize.x);
        float IntersectY = abs(DeltaY) - (OtherHalfSize.y + ThisHalfSize.y);

        if (IntersectX < 0.0f && IntersectY < 0.0f) {

            push = std::min(std::max(push, 0.0f), 1.0f);

            if (IntersectX > IntersectY) {

                if (DeltaX > 0.0f) {

                    Move(IntersectX * (1.0f - push), 0.0f, false);
                    OtherObject.Move(-IntersectX * push, 0.0f, true);

                } else {

                    Move(-IntersectX * (1.0f - push), 0.0f, false);
                    OtherObject.Move(IntersectX * push, 0.0f, true);

                }

            } else {

                if (DeltaY > 0.0f) {

                    Move(0.0f, IntersectY * (1.0f - push), false);
                    OtherObject.Move(0.0f, -IntersectY * push, true);

                } else {

                    Move(0.0f, -IntersectY * (1.0f - push), false);
                    OtherObject.Move(0.0f, IntersectY * push, true);

                }

            }

            return true;

        }

    } else {

        sf::Vector2f OtherPosition = OtherObject.GetPosition();
        sf::Vector2f OtherHalfSize = OtherObject.GetHalfSize();
        sf::Vector2f ThisPosition = GetPosition();
        sf::Vector2f ThisHalfSize = GetHalfSize();

        float DeltaX = OtherPosition.x - ThisPosition.x;
        float DeltaY = OtherPosition.y - ThisPosition.y;

        float IntersectX = abs(DeltaX) - (OtherHalfSize.x + ThisHalfSize.x);
        float IntersectY = abs(DeltaY) - (OtherHalfSize.y + ThisHalfSize.y);

        if (IntersectX < 0.0f && IntersectY < 0.0f) {

            push = std::min(std::max(push, 0.0f), 1.0f);

            if (IntersectX > IntersectY) {

                if (DeltaX > 0.0f) {

                    Move(IntersectX * (1.0f - push), 0.0f, false);
                    OtherObject.Move(-IntersectX * push, 0.0f, false);

                } else {

                    Move(-IntersectX * (1.0f - push), 0.0f, false);
                    OtherObject.Move(IntersectX * push, 0.0f, false);

                }

            } else {

                if (DeltaY > 0.0f) {

                    Move(0.0f, IntersectY * (1.0f - push), false);
                    OtherObject.Move(0.0f, -IntersectY * push, false);

                } else {

                    Move(0.0f, -IntersectY * (1.0f - push), false);
                    OtherObject.Move(0.0f, IntersectY * push, false);

                }

            }

            return true;

        }

    }

    return false;

}

sf::Vector2f Collider::GetPosition() {
    return sprite.getPosition();
}

sf::Vector2f Collider::GetExtendedColliderPosition() {
    return collider.getPosition();
}

sf::Vector2f Collider::GetHalfSize() {
    return sf::Vector2f(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
}

sf::Vector2f Collider::GetExtendedColliderHalfSize() {
    return sf::Vector2f(collider.getGlobalBounds().width / 2, collider.getGlobalBounds().height / 2);
}

void Collider::Move(float dx, float dy, bool ExtendedCollider) {
    sprite.move(dx, dy);
    if (ExtendedCollider) 
        collider.move(dx, dy);
}