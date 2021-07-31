#include "../headers/Animation.h"

Animation::Animation() {}

Animation::Animation(sf::Sprite sprite, float SwitchTime, sf::Vector2u ImageCount) {

        this->sprite = sprite;
        this->SwitchTime = SwitchTime;
        this->ImageCount = ImageCount;
        TotalTime = 0.0f;
        CurrentImage.x = 0;

        UvRect.width = this->sprite.getTexture()->getSize().x / float(ImageCount.x);
        UvRect.height = this->sprite.getTexture()->getSize().y / float(ImageCount.y);

}

void Animation::Update(int Row, float deltaTime, bool FaceRight) {
        
        CurrentImage.y = Row;
        TotalTime += deltaTime;

        if (TotalTime >= SwitchTime) {

            TotalTime -= SwitchTime;
            CurrentImage.x++;

            if (CurrentImage.x >= ImageCount.x) 
                CurrentImage.x = 0;
            
        } 

        if (FaceRight) {

            UvRect.left = CurrentImage.x * abs(UvRect.width);
            UvRect.width = abs(UvRect.width);

        } else {

            UvRect.left = CurrentImage.x * abs(UvRect.width) + abs(UvRect.width);
            UvRect.width = -abs(UvRect.width);

        }
        
        UvRect.top = CurrentImage.y * UvRect.height;

}

sf::IntRect Animation::GetIntRect() {
        return UvRect;
}
