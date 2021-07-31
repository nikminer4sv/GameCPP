#include <SFML/Graphics.hpp>
#ifndef ANIMATION_H
#define ANIMATION_H

class Animation {
private:

    float SwitchTime;
    float TotalTime;
    sf::Sprite sprite;
    sf::Vector2u ImageCount;
    sf::Vector2u CurrentImage;
    sf::IntRect UvRect;


public:

    Animation();

    Animation(sf::Sprite sprite, float SwitchTime, sf::Vector2u ImageCount);

    void Update(int Row, float deltaTime, bool FaceRight);

    sf::IntRect GetIntRect();

};

#endif