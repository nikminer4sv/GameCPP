#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <string>
#include <stdlib.h>
#include <ctime>
#include <math.h>

#include "headers/Enums.h"
#include "headers/Position2D.h"
#include "headers/Collider.h"
#include "headers/Animation.h"
#include "headers/Cell.h"
#include "headers/Character.h"
#include "headers/Projectile.h"
#include "headers/Object.h"

// Используемые пространства имен
using namespace std;
using namespace sf;

// Переменные, содержащие загружаемые материалы
Texture CharacterTileset;
Texture GroundTileset;
Texture ResourcesTileset;
Texture ResourceBackgroundTexture;
Texture StoneWallTileset;
Texture BuildBackgroundTexture;
Texture BuildButtonsTileset;
Sprite CharacterSprite;
Sprite ResourcesStoneSprite;
Sprite ResourcesWoodSprite;
Sprite ResourcesGoldSprite;
Sprite ResourcesIronSprite;
Sprite ResourceBackgroundSprite;
Sprite StoneWall_pillar;
Sprite BuildBackgroundSprite;
Sprite StoneWallBuildButton;
Sprite StoneRoadBuildButton;
Sprite StoneFloorBuildButton;
Sprite EmptyBuildButton;
vector<Sprite> StoneWallSprites(1);
vector<Sprite> GrassSprites(32);
vector<Sprite> RoadSprites(32);
Texture StoneWall;
Sprite StoneWallSprite;
Image icon;
SoundBuffer WindBuffer;
SoundBuffer WalkBuffer;
SoundBuffer BuildBuffer;
Sound WindSound;
Sound WalkSound;
Sound BuildSound;
CircleShape arrow;

Text WoodCounter;
Text StoneCounter;
Text IronCounter;
Text GoldCounter;

Font MainFont;

// GUI
Sprite RoadtypeBackground;
Texture RoadtypeBackgroundTexture;

// Константы
const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;
const int VIEW_WIDTH = 850;
const int VIEW_HEIGHT = 450;
const int WORLD_WIDTH = 30;
const int WORLD_HEIGHT = 30;
const int TEXTURE_SIZE = 32;
const int CHARACTER_SIZE = 32;
const int CHARACTER_SCALE_FACTOR = 2;
const int CHARACTER_SPEED = 160;
const string WINDOW_TITLE = "Game";
const string TEXTURES_PATH = "source/textures/";
const string ICONS_PATH = "source/icons/";
const string GUI_PATH = "source/GUI/";
const string AUDIO_PATH = "source/audio/";
const string FONTS_PATH = "source/fonts/";
const bool isGenerator = true;

int WoodValue = 200;
int StoneValue = 200;
int IronValue = 200;
int GoldValue = 200;

void LoadSource();
int GetRandomNumber(int min, int max);
vector<vector<Cell>> DefaultMap();

class GUI {
private:

    vector<Sprite> GUIObjects;
    vector<Text> TextObjects;


public:

    GUI(int CurrentRoad);

    vector<Sprite> GetObjects();

    vector<Text> GetTextObjects();

    void UpdateRoad(int CurrentRoad);

    void UpdateTextData();

};

// Точка входа
int main() {

    // Зерно рандома
    srand(static_cast<unsigned int>(time(0)));

    // Загрузка материалов игры
    LoadSource();

    // Создание карты игры
    vector<vector<Cell>> GameField = DefaultMap();
    vector<vector<Object>> ObjectsField(WORLD_HEIGHT, vector<Object>(WORLD_WIDTH, Object()));

    // Создание персонажа
    Character character(CharacterSprite, Position2D(static_cast<float>(WORLD_WIDTH*TEXTURE_SIZE/2), static_cast<float>(WORLD_HEIGHT*TEXTURE_SIZE/2)), CHARACTER_SPEED);

    // Создание анимации персонажа
    Animation CharacterAnimation(character.GetSprite(), 0.07f, Vector2u(10,10));

    // Создание камеры персонажа
    View CharacterView(FloatRect((WORLD_WIDTH*TEXTURE_SIZE-VIEW_WIDTH)/2, (WORLD_HEIGHT*TEXTURE_SIZE-VIEW_HEIGHT)/2, VIEW_WIDTH, VIEW_HEIGHT));

    // Создание главного окна игры
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE, Style::Fullscreen);

    // Конфигурация главного окна игры
    window.setFramerateLimit(61);
    window.setView(CharacterView);
    window.setIcon(512,512,icon.getPixelsPtr());

    // Нужно для сохранение выбранного типа дороги для генератора
    int current_road_type = 0;  

    Object CurrentBuildObject = Object(StoneWall_pillar, 0, 10, Resources::STONE);

    // Инициализация переменной,хранящей время одного кадра
    float deltaTime = 0.0f;
    
    // Создание таймера
    Clock clock;

    // Инициализация переменной, хранящей направление персонажа
    bool LastFaceRight = true;

    // Запуск звука ветра на фоне
    WindSound.play();

    vector<Projectile> LiveProjectTiles(0);
    GUI gui(current_road_type);
    bool isTabPressed = false;

    // Создание главного цикла игры
    while(window.isOpen()) {

        // Запись времени одного кадра в переменную
        deltaTime = clock.restart().asSeconds();

        // Переменная, необходимая для движения персонажа
        bool isMoving = false;

        // Переменная для отслеживания событий
        Event event;

        // Цикл, обрабатывающий события
        while(window.pollEvent(event)) {

            // Условия закрытия игры
            if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) 
                window.close();
            
            // Смена типа дороги
            if (Keyboard::isKeyPressed(Keyboard::Tab)) {

                if (isTabPressed != true) {

                    if (current_road_type + 1 == 32) 
                        current_road_type = 0;
                    else 
                        current_road_type++;

                    gui.UpdateRoad(current_road_type);

                    isTabPressed = true;

                }
                
            } else {

                isTabPressed = false;

            }

            // Строительство
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Right) {

                if (isGenerator) {

                    // Переменные строки и колонки игрового поля
                    int col, row;

                    // mapPixelToCoords - преобразование координат вьюпорта в глобальные
                    col = window.mapPixelToCoords(Mouse::getPosition(window)).x / TEXTURE_SIZE;
                    row = window.mapPixelToCoords(Mouse::getPosition(window)).y / TEXTURE_SIZE;

                    // Проверка на повторяемость
                    if (GameField[row][col].GetSprite().getTextureRect() != RoadSprites[current_road_type].getTextureRect()) {

                        // Замена клетки поля
                        GameField[row][col].ChangeCell(RoadSprites[current_road_type], CellType::ROAD);     

                        // Воспроизведение звука строительства
                        BuildSound.play();

                    }

                } 

            }

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left){

                // Переменные строки и колонки игрового поля
                int col, row;

                // mapPixelToCoords - преобразование координат вьюпорта в глобальные
                col = window.mapPixelToCoords(Mouse::getPosition(window)).x / TEXTURE_SIZE;
                row = window.mapPixelToCoords(Mouse::getPosition(window)).y / TEXTURE_SIZE;

                // Проверка на повторяемость
                if (ObjectsField[row][col].isFiller()) {

                    Resources CurrentResourceTypeForBuild = ObjectsField[row][col].GetResourceTypeForBuild();

                    int* CurrentValue;

                    if (CurrentResourceTypeForBuild == Resources::WOOD) {
                        CurrentValue = &WoodValue;
                    } else if (CurrentResourceTypeForBuild == Resources::STONE) {
                        CurrentValue = &StoneValue;
                    } else if (CurrentResourceTypeForBuild == Resources::IRON) {
                        CurrentValue = &IronValue;
                    } else {
                        CurrentValue = &GoldValue;
                    }

                    CurrentValue = &StoneValue;

                    if (CurrentBuildObject.GetCost() <= *CurrentValue) {

                        *CurrentValue -= CurrentBuildObject.GetCost();

                        // Замена клетки поля
                        ObjectsField[row][col] = CurrentBuildObject;     
                        ObjectsField[row][col].SetPosition(Vector2f(col * TEXTURE_SIZE + TEXTURE_SIZE / 2, row * TEXTURE_SIZE + TEXTURE_SIZE / 2));

                        // Воспроизведение звука строительства
                        BuildSound.play();


                    }

                }
                
            }

        }

        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Middle) {

            Position2D CurrentPosition(character.GetSpriteRef()->getPosition().x, character.GetSpriteRef()->getPosition().y);
            Position2D MousePosition((float)window.mapPixelToCoords(Mouse::getPosition(window)).x, (float)window.mapPixelToCoords(Mouse::getPosition(window)).y);
            LiveProjectTiles.push_back(Projectile(CurrentPosition, MousePosition, 600));

        }

        // Переменная, хранящая информацию о том, менялась ли анимация в этой итерации цикла
        bool updated = false;

        // Движение в право
        if (Keyboard::isKeyPressed(Keyboard::D)) {
            
            // Проверка на выход за границы мира
            if (character.GetSpriteRef()->getPosition().x + deltaTime*CHARACTER_SPEED + CHARACTER_SIZE * CHARACTER_SCALE_FACTOR / 2<= WORLD_WIDTH*TEXTURE_SIZE) {

                // Движение персонажа
                character.Move(MoveDirections::RIGHT, deltaTime);

                // Проверка на повторяемость анимации
                if (updated == false) { 
                    CharacterAnimation.Update(2, deltaTime, true); 
                    updated = true;
                }

                // Нужно для корректной работы камеры
                if (CharacterView.getCenter().x + VIEW_WIDTH/2 + deltaTime*CHARACTER_SPEED <= WORLD_WIDTH*TEXTURE_SIZE) {
                    if (!(character.GetSpriteRef()->getPosition().x < CharacterView.getCenter().x)) {
                        CharacterView.move(deltaTime*CHARACTER_SPEED, 0.f);
                        window.setView(CharacterView);
                    }
                }

                // Повернуть персонажа в право и сообщить, что он двигается
                LastFaceRight = true;
                isMoving = true;

            }
        } 

        if (Keyboard::isKeyPressed(Keyboard::S)) {

            // Проверка на выход за границы мира
            if (character.GetSpriteRef()->getPosition().y + deltaTime*CHARACTER_SPEED + CHARACTER_SIZE * CHARACTER_SCALE_FACTOR / 2 <= WORLD_HEIGHT*TEXTURE_SIZE) {

                float StartPosY = character.GetSpriteRef()->getPosition().y;

                // Движение персонажа
                character.Move(MoveDirections::BOTTOM, deltaTime);

                // Проверка на повторяемость анимации
                if (updated == false) {
                    CharacterAnimation.Update(2, deltaTime, LastFaceRight);
                    updated = true;
                }

                // Нужно для корректной работы камеры
                if (CharacterView.getCenter().y + VIEW_HEIGHT/2 + deltaTime*CHARACTER_SPEED <= WORLD_HEIGHT*TEXTURE_SIZE) {
                    if (!(character.GetSpriteRef()->getPosition().y < CharacterView.getCenter().y)) {
                        if (character.GetSpriteRef()->getPosition().y != StartPosY) {
                            CharacterView.move(0.f, deltaTime*CHARACTER_SPEED);
                            window.setView(CharacterView);
                        }
                    }
                }

                // Сообщить, что персонаж двигается
                isMoving = true;
            }
        } 

        if (Keyboard::isKeyPressed(Keyboard::A)) {

            // Проверка на выход за границы мира
            if (character.GetSpriteRef()->getPosition().x - CHARACTER_SIZE * CHARACTER_SCALE_FACTOR / 2 - deltaTime*CHARACTER_SPEED  >= 0) {

                // Движение персонажа
                character.Move(MoveDirections::LEFT, deltaTime);

                // Проверка на повторяемость анимации
                if (updated == false) {
                    CharacterAnimation.Update(2, deltaTime, false);
                    updated = true;
                }

                // Нужно для корректной работы камеры
                if (CharacterView.getCenter().x - VIEW_WIDTH/2 - deltaTime*CHARACTER_SPEED >= 0) {

                    if (!(character.GetSpriteRef()->getPosition().x > CharacterView.getCenter().x)) {
                            CharacterView.move(-deltaTime*CHARACTER_SPEED, 0.f);
                            window.setView(CharacterView);
                    }

                }

                // Повернуть персонажа в право и сообщить, что он двигается
                LastFaceRight = false;
                isMoving = true;

            }
        } 

        if (Keyboard::isKeyPressed(Keyboard::W)) {

            // Проверка на выход за границы мира
            if (character.GetSpriteRef()->getPosition().y - deltaTime*CHARACTER_SPEED - CHARACTER_SIZE * CHARACTER_SCALE_FACTOR / 2 >= 0) {

                // Движение персонажа
                character.Move(MoveDirections::TOP, deltaTime);

                // Проверка на повторяемость анимации
                if (updated == false) {
                    CharacterAnimation.Update(2, deltaTime, LastFaceRight);
                    updated = true;
                }

                // Нужно для корректной работы камеры
                if (CharacterView.getCenter().y - VIEW_HEIGHT/2 - deltaTime*CHARACTER_SPEED >= 0) {
                    if (!(character.GetSpriteRef()->getPosition().y > CharacterView.getCenter().y)) {
                        CharacterView.move(0.f, -deltaTime*CHARACTER_SPEED);
                        window.setView(CharacterView);
                    }
                }

                // Сообщить, что персонаж двигается
                isMoving = true;
            }
        }

        // Отрисовка игрового мира
        for (int i = 0; i < WORLD_HEIGHT; i++) {
            for (int j = 0; j < WORLD_WIDTH; j++) {
                window.draw(GameField[i][j].GetSprite());
            }
        }

        for (int i = 0; i < WORLD_HEIGHT; i++) {
            for (int j = 0; j < WORLD_WIDTH; j++) {
                if (!ObjectsField[i][j].isFiller())
                    window.draw(ObjectsField[i][j].GetSprite());
            }
        }

        // Проверка на то, стоит ли персонаж или двигается
        if (isMoving == false) {
            CharacterAnimation.Update(0, deltaTime, LastFaceRight);
            if (WalkSound.getStatus() == Sound::Playing)
                WalkSound.stop();
        } else {
            if (WalkSound.getStatus() != Sound::Playing) {
                WalkSound.play();
            }
        }

        for (int i = 0; i < LiveProjectTiles.size(); i++) {
            LiveProjectTiles[i].Update(deltaTime);
            window.draw(LiveProjectTiles[i].GetSprite());
        }

        vector<Sprite> GUIObjects = gui.GetObjects();
        for (int i = 0; i < GUIObjects.size(); i++) {

            Sprite temp = GUIObjects[i];
            
            temp.setPosition(CharacterView.getCenter().x - VIEW_WIDTH / 2 + temp.getPosition().x, CharacterView.getCenter().y - VIEW_HEIGHT / 2 + temp.getPosition().y);
            
            window.draw(temp);

        }

        vector<Text> TextObjects = gui.GetTextObjects();
        gui.UpdateTextData();
        for (int i = 0; i < TextObjects.size(); i++) {

            Text temp = TextObjects[i];
            
            temp.setPosition(CharacterView.getCenter().x - VIEW_WIDTH / 2 + temp.getPosition().x, CharacterView.getCenter().y - VIEW_HEIGHT / 2 + temp.getPosition().y);

            window.draw(temp);

        }

        Collider CharacterCollider = character.GetCollider();

        for (int i = 0; i < WORLD_HEIGHT; i++) {
            for (int j = 0; j < WORLD_WIDTH; j++) {
                if (!ObjectsField[i][j].isFiller()) {
                    if (ObjectsField[i][j].isColliding()) 
                        ObjectsField[i][j].GetCollider().CheckCollision(CharacterCollider, 1.0f, true);
                }
            }
        }

        // Обновление спрайта персонажа и его отрисовка
        character.GetSpriteRef()->setTextureRect(CharacterAnimation.GetIntRect());
        window.draw(character.GetSprite());

        // Отрисовка окна
        window.display();

    }

}

// Загрузка ресурсов игры
void LoadSource() {

    MainFont.loadFromFile(FONTS_PATH + "MainFont.ttf");

    // Загрузка тайлсета с землёй и дорогой
    GroundTileset.loadFromFile(TEXTURES_PATH + "GroundTileset.png");

    // Заполение вектора спрайтами земли
    for (int i = 0; i < 32; i++) {
        GrassSprites[i].setTexture(GroundTileset);
        GrassSprites[i].setTextureRect(IntRect((i % 8)*TEXTURE_SIZE,(i / 8)*TEXTURE_SIZE,32,32));
    }

    // Заполение вектора спрайтами дороги
    for (int i = 0; i < 32; i++) {
        RoadSprites[i].setTexture(GroundTileset);
        RoadSprites[i].setTextureRect(IntRect((i % 8)*TEXTURE_SIZE,(i / 8)*TEXTURE_SIZE+128,32,32));
    }

    StoneWallTileset.loadFromFile(TEXTURES_PATH + "StoneWallTileset.png");
    StoneWall_pillar.setTexture(StoneWallTileset);
    StoneWall_pillar.setTextureRect(IntRect(96,32,32,32));

    // Настройка персонажа
    CharacterTileset.loadFromFile(TEXTURES_PATH + "Character.png");
    CharacterSprite.setTexture(CharacterTileset);
    CharacterSprite.setTextureRect(IntRect(0,64,32,32));
    CharacterSprite.scale(CHARACTER_SCALE_FACTOR, CHARACTER_SCALE_FACTOR);
    

    // Загрузка иконки игры
    icon.loadFromFile(ICONS_PATH + "GameIcon.png");

    // Загрузка и конфигурация звуков ветра
    WindBuffer.loadFromFile(AUDIO_PATH + "WindSound.wav");
    WindSound.setBuffer(WindBuffer);
    WindSound.setLoop(true);
    WindSound.setVolume(7.f);
    
    // Загрузка и конфигурация звуков ветра
    WalkBuffer.loadFromFile(AUDIO_PATH + "WalkSound.wav");
    WalkSound.setBuffer(WalkBuffer);

    // Загрузка и конфигурация звуков строительства
    BuildBuffer.loadFromFile(AUDIO_PATH + "BuildSound.wav");
    BuildSound.setBuffer(BuildBuffer);

    arrow.setRadius(1.f);

    // GUI
    RoadtypeBackgroundTexture.loadFromFile(GUI_PATH + "RoadtypeBackground.png");
    RoadtypeBackground.setTexture(RoadtypeBackgroundTexture);
    RoadtypeBackground.setScale(0.25, 0.25);

    ResourcesTileset.loadFromFile(GUI_PATH + "ResourcesTileset.png");
    ResourcesStoneSprite.setTexture(ResourcesTileset);
    ResourcesWoodSprite.setTexture(ResourcesTileset);
    ResourcesGoldSprite.setTexture(ResourcesTileset);
    ResourcesIronSprite.setTexture(ResourcesTileset);
    ResourcesStoneSprite.setTextureRect(IntRect(0,0,24,24));
    ResourcesWoodSprite.setTextureRect(IntRect(72,0,24,24));
    ResourcesGoldSprite.setTextureRect(IntRect(24,0,24,24));
    ResourcesIronSprite.setTextureRect(IntRect(48,0,24,24));

    ResourceBackgroundTexture.loadFromFile(GUI_PATH + "ResourceBackground.png");
    ResourceBackgroundSprite.setTexture(ResourceBackgroundTexture);
    StoneCounter.setFont(MainFont);
    StoneCounter.setString(to_string(StoneValue));
    StoneCounter.setScale(0.45,0.45);
    WoodCounter.setFont(MainFont);
    WoodCounter.setString(to_string(WoodValue));
    WoodCounter.setScale(0.45,0.45);
    IronCounter.setFont(MainFont);
    IronCounter.setString(to_string(IronValue));
    IronCounter.setScale(0.45,0.45);
    GoldCounter.setFont(MainFont);
    GoldCounter.setString(to_string(GoldValue));
    GoldCounter.setScale(0.45,0.45);

    BuildBackgroundTexture.loadFromFile(GUI_PATH + "BuildBackground.png");
    BuildBackgroundSprite.setTexture(BuildBackgroundTexture);
    BuildBackgroundSprite.setScale(1.5, 1.5);

    BuildButtonsTileset.loadFromFile(GUI_PATH + "BuildButtons.png");
    StoneWallBuildButton.setTexture(BuildButtonsTileset);
    StoneWallBuildButton.setTextureRect(IntRect(0,0,20,20));
    StoneWallBuildButton.setScale(1.5,1.5);
    StoneFloorBuildButton.setTexture(BuildButtonsTileset);
    StoneFloorBuildButton.setTextureRect(IntRect(20,0,20,20));
    StoneFloorBuildButton.setScale(1.5,1.5);
    StoneRoadBuildButton.setTexture(BuildButtonsTileset);
    StoneRoadBuildButton.setTextureRect(IntRect(40,0,20,20));
    StoneRoadBuildButton.setScale(1.5,1.5);
    EmptyBuildButton.setTexture(BuildButtonsTileset);
    EmptyBuildButton.setTextureRect(IntRect(60,0,20,20));
    EmptyBuildButton.setScale(1.5,1.5);

    // StoneWall
    StoneWall.loadFromFile(TEXTURES_PATH + "StoneWall.png");
    StoneWallSprite.setTexture(StoneWall);
    StoneWallSprite.setTextureRect(IntRect(0,0,32,32));

}

// Метод, возвращающий случайное число в заданном диапазоне
int GetRandomNumber(int min, int max) {

    // Без понятия как это работает:)
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0); 
    return static_cast<int>(rand() * fraction * (max - min + 1) + min);

}

// Генерация мира
vector<vector<Cell>> DefaultMap() {

    // Создание вектора, содержащего игровое поле
    vector<vector<Cell>> GameField(WORLD_HEIGHT, vector<Cell>(WORLD_WIDTH, Cell()));

    // Заполнение вектора объектами по умолчанию
    for (int i = 0; i < WORLD_HEIGHT; i++) {
        for (int j = 0; j < WORLD_WIDTH; j++) {
            int rnumber = GetRandomNumber(0,31);
            GameField[i][j].ChangeCell(GrassSprites[rnumber], CellType::GRASS);
            GameField[i][j].SetPosition(j * TEXTURE_SIZE, i * TEXTURE_SIZE);
        }
    }

    // Возврат игрового поля
    return GameField;

}

GUI::GUI(int CurrentRoad) {

    GUIObjects = vector<Sprite>(30);
    //GUIObjects.push_back(RoadtypeBackground);
    //GUIObjects[0].setPosition(VIEW_WIDTH - 10 - GUIObjects[0].getGlobalBounds().width, VIEW_HEIGHT - 10 - GUIObjects[0].getGlobalBounds().height);
    //GUIObjects.push_back(RoadSprites[CurrentRoad]);
    //GUIObjects[1].scale(1.5, 1.5);
    //GUIObjects[1].setPosition(VIEW_WIDTH - 18 - GUIObjects[1].getGlobalBounds().width, VIEW_HEIGHT - 18 - GUIObjects[1].getGlobalBounds().height);
    GUIObjects[0] = ResourceBackgroundSprite;
    GUIObjects[0].setPosition(5,5);
    GUIObjects[3] = ResourcesWoodSprite;
    GUIObjects[3].setPosition(6,6);
    GUIObjects[4] = ResourceBackgroundSprite;
    GUIObjects[4].setPosition(5,37);
    GUIObjects[5] = ResourcesStoneSprite;
    GUIObjects[5].setPosition(6,38);
    GUIObjects[6] = ResourceBackgroundSprite;
    GUIObjects[6].setPosition(5,69);
    GUIObjects[7] = ResourcesIronSprite;
    GUIObjects[7].setPosition(6,70);
    GUIObjects[8] = ResourceBackgroundSprite;
    GUIObjects[8].setPosition(5,101);
    GUIObjects[9] = ResourcesGoldSprite;
    GUIObjects[9].setPosition(6,102);
    GUIObjects[10] = BuildBackgroundSprite;
    GUIObjects[10].setPosition(VIEW_WIDTH - BuildBackgroundSprite.getGlobalBounds().width, VIEW_HEIGHT - BuildBackgroundSprite.getGlobalBounds().height);
    GUIObjects[11] = StoneWallBuildButton;
    GUIObjects[11].setPosition(GUIObjects[10].getPosition().x + 10.5, GUIObjects[10].getPosition().y + 7.5);
    GUIObjects[12] = StoneFloorBuildButton;
    GUIObjects[12].setPosition(GUIObjects[10].getPosition().x + 10.5 + 37.5, GUIObjects[10].getPosition().y + 7.5);
    GUIObjects[13] = StoneRoadBuildButton;
    GUIObjects[13].setPosition(GUIObjects[10].getPosition().x + 10.5 + 75, GUIObjects[10].getPosition().y + 7.5);
    GUIObjects[14] = EmptyBuildButton;
    GUIObjects[14].setPosition(GUIObjects[10].getPosition().x + 10.5, GUIObjects[10].getPosition().y + 42);
    GUIObjects[15] = EmptyBuildButton;
    GUIObjects[15].setPosition(GUIObjects[10].getPosition().x + 10.5 + 37.5, GUIObjects[10].getPosition().y + 42);
    GUIObjects[16] = EmptyBuildButton;
    GUIObjects[16].setPosition(GUIObjects[10].getPosition().x + 10.5 + 75, GUIObjects[10].getPosition().y + 42);

    TextObjects.push_back(WoodCounter);
    TextObjects[0].setPosition(33,11);
    TextObjects.push_back(StoneCounter);
    TextObjects[1].setPosition(33,43);
    TextObjects.push_back(IronCounter);
    TextObjects[2].setPosition(33,75);
    TextObjects.push_back(GoldCounter);
    TextObjects[3].setPosition(33,107);

}

vector<Sprite> GUI::GetObjects() {
    return GUIObjects;
}

vector<Text> GUI::GetTextObjects() {
    return TextObjects;
}

void GUI::UpdateRoad(int CurrentRoad) {
    GUIObjects[1].setTextureRect(RoadSprites[CurrentRoad].getTextureRect());
}

void GUI::UpdateTextData() {
    TextObjects[0].setString(to_string(WoodValue));
    TextObjects[1].setString(to_string(StoneValue));
    TextObjects[2].setString(to_string(IronValue));
    TextObjects[3].setString(to_string(GoldValue));
}