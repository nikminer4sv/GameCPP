#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <string>
#include <stdlib.h>
#include <ctime>
#include <math.h>

// Используемые пространства имен
using namespace std;
using namespace sf;

// Переменные, содержащие загружаемые материалы
Texture CharacterTileset;
Texture GroundTileset;
Sprite CharacterSprite;
vector<Sprite> GrassSprites(32);
vector<Sprite> RoadSprites(32);
Image icon;
SoundBuffer WindBuffer;
SoundBuffer WalkBuffer;
SoundBuffer BuildBuffer;
Sound WindSound;
Sound WalkSound;
Sound BuildSound;
CircleShape arrow;

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
const bool isGenerator = true;


// Перечисление типов клетки
enum CellType {
    GRASS,
    ROAD
};

// Перечисление направлений движения персонажа
enum MoveDirections {
    TOP,
    RIGHT,
    BOTTOM,
    LEFT
};

// Класс позиции объекта
class Position2D {
private:
    int x;
    int y;
    float xf;
    float yf;

public:

    Position2D() {
        x = 0;
        y = 0;
        xf = 0;
        yf = 0;
    }

    Position2D(int x, int y) {
        this->x = x;
        this->y = y;
    }

    Position2D(float xf, float yf) {
        this->xf = xf;
        this->yf = yf;
    }

    int GetX() {
        return x;
    }

    int GetY() {
        return y;
    }

    void SetX(int x) {
        this->x = x;
    }

    void SetY(int y) {
        this->y = y;
    }

    void SetXY(int x, int y) {
        this->x = x;
        this->y = y;
    }

    float GetXF() {
        return xf;
    }

    float GetYF() {
        return yf;
    }

    void SetXF(float xf) {
        this->xf = xf;
    }

    void SetYF(float yf) {
        this->yf = yf;
    }

    void SetXYF(float xf, float yf) {
        this->xf = xf;
        this->yf = yf;
    }

};

// Класс, отвечающий за клетку игрового поля
class Cell {
private:
    CellType type;
    Sprite sprite;
    Position2D position;

public:
    Cell() {
        type = GRASS;
        sprite = GrassSprites[0];
        sprite.setPosition(0,0);
    }

    Cell(CellType type) {
        
        this->type = type;

        if (type == GRASS) {
            sprite = GrassSprites[0];
        } else if (type == ROAD) {
            sprite = RoadSprites[0];
        }

        sprite.setPosition(0,0);
    }

    Position2D GetPosition() {
        return position;
    }

    void SetPosition(int x, int y) {
        position.SetXY(x,y);
        sprite.setPosition(position.GetX(), position.GetY());
    }

    Sprite GetSprite() {
        return sprite;
    }
    
    void ChangeCell(Sprite sprite, CellType type) {
        sprite.setPosition(position.GetX(), position.GetY());
        SetSprite(sprite);
        SetType(type);
    }

    void SetSprite(Sprite sprite) {
        this->sprite = sprite;
    }

    void SetType(CellType type) {
        this->type = type;
    }

    CellType GetType() {
        return type;
    }

};

// Загрузка ресурсов игры
void LoadSource() {

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

    // Настройка персонажа
    CharacterTileset.loadFromFile(TEXTURES_PATH + "Character.png");
    CharacterSprite.setTexture(CharacterTileset);
    CharacterSprite.setPosition(WINDOW_WIDTH/2 - CHARACTER_SIZE/2, WINDOW_HEIGHT/2 - CHARACTER_SIZE/2);
    CharacterSprite.setTextureRect(IntRect(0,64,32,32));
    CharacterSprite.scale(CHARACTER_SCALE_FACTOR, CHARACTER_SCALE_FACTOR);

    // Загрузка иконки игры
    icon.loadFromFile("source/icons/GameIcon.png");

    // Загрузка и конфигурация звуков ветра
    WindBuffer.loadFromFile("source/audio/WindSound.wav");
    WindSound.setBuffer(WindBuffer);
    WindSound.setLoop(true);
    WindSound.setVolume(7.f);
    
    // Загрузка и конфигурация звуков ветра
    WalkBuffer.loadFromFile("source/audio/WalkSound.wav");
    WalkSound.setBuffer(WalkBuffer);

    // Загрузка и конфигурация звуков строительства
    BuildBuffer.loadFromFile("source/audio/BuildSound.wav");
    BuildSound.setBuffer(BuildBuffer);

    arrow.setRadius(1.f);

    // GUI
    RoadtypeBackgroundTexture.loadFromFile("source/GUI/RoadtypeBackground.png");
    RoadtypeBackground.setTexture(RoadtypeBackgroundTexture);
    RoadtypeBackground.setScale(0.25, 0.25);

}

// Класс персонажа
class Character {
private:
    Sprite sprite;
    Position2D position;
    int health = 100;
    IntRect intrect;
    MoveDirections side = RIGHT;

public:
    Character() {}

    Character(Sprite spritearg, Position2D positionarg):position(positionarg), sprite(spritearg) {
        sprite.setPosition(position.GetXF(), position.GetYF());
        intrect = IntRect(0,64,32,32);
    }

    Sprite GetSprite() {
        return sprite;
    }

    Sprite* GetSpriteRef() {
        return &sprite;
    }

    Position2D GetPosition() {
        return position;
    }

    void Move(MoveDirections direction, float deltaTime) {

        if (direction == RIGHT) {
            position.SetXF(position.GetXF() + deltaTime*CHARACTER_SPEED);
            sprite.setPosition(position.GetXF(), position.GetYF());
            if (side == LEFT) {
                intrect.width *= -1;
                intrect.left -= 32;
                sprite.setTextureRect(intrect);
            }
            side = RIGHT;
        } else if (direction == BOTTOM) {
            position.SetYF(position.GetYF() + deltaTime*CHARACTER_SPEED);
            sprite.setPosition(position.GetXF(), position.GetYF());
        } else if (direction == LEFT) {
            sprite.setTextureRect(IntRect(32,64,-32,32));
            position.SetXF(position.GetXF() - deltaTime*CHARACTER_SPEED);
            sprite.setPosition(position.GetXF(), position.GetYF());
            if (side == RIGHT) {
                intrect.width *= -1;
                intrect.left += 32;
                sprite.setTextureRect(intrect);
            }
            side = LEFT;
        } else if (direction == TOP) {
            position.SetYF(position.GetYF() - deltaTime*CHARACTER_SPEED);
            sprite.setPosition(position.GetXF(), position.GetYF());
        }

    }

};

// Класс, отвечающий за анимации
class Animation {
private:
    float SwitchTime;
    Sprite sprite;
    Vector2u ImageCount;
    Vector2u CurrentImage;
    IntRect UvRect;
    float TotalTime;

public:
    Animation() {}

    Animation(Sprite sprite, float SwitchTime, Vector2u ImageCount) {
        this->sprite = sprite;
        this->SwitchTime = SwitchTime;
        this->ImageCount = ImageCount;
        TotalTime = 0.0f;
        CurrentImage.x = 0;

        UvRect.width = this->sprite.getTexture()->getSize().x / float(ImageCount.x);
        UvRect.height = this->sprite.getTexture()->getSize().y / float(ImageCount.y);

    }

    void Update(int Row, float deltaTime, bool FaceRight) {
        
        CurrentImage.y = Row;
        TotalTime += deltaTime;

        if (TotalTime >= SwitchTime) {

            TotalTime -= SwitchTime;
            CurrentImage.x++;

            if (CurrentImage.x >= ImageCount.x) {
                CurrentImage.x = 0;
            }

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

    IntRect GetIntRect() {
        return UvRect;
    }

};

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
            GameField[i][j].ChangeCell(GrassSprites[rnumber], GRASS);
            GameField[i][j].SetPosition(j * TEXTURE_SIZE, i * TEXTURE_SIZE);
        }
    }

    // Возврат игрового поля
    return GameField;

}

class ProjectTile {
private:
    Sprite sprite;
    Position2D StartPosition;
    Position2D CurrentPosition;
    Position2D MousePosition;
    CircleShape arrow;
    int speed;
    float sin;
    float cos;

public:
    ProjectTile() {}

    ProjectTile(Sprite sprite, Position2D StartPosition, Position2D MousePosition, int speed) {

        this->sprite = sprite;
        this->StartPosition = StartPosition;
        this->MousePosition = MousePosition;
        this->speed = speed;
        sprite.setPosition(StartPosition.GetXF(), StartPosition.GetYF());
        sin = abs(StartPosition.GetYF() - MousePosition.GetYF()) / abs(StartPosition.GetXF() - MousePosition.GetXF());
        cos = abs(StartPosition.GetXF() - MousePosition.GetXF()) / abs(StartPosition.GetYF() - MousePosition.GetYF());

    }

    ProjectTile(Position2D StartPosition, Position2D MousePosition, int speed) {

        arrow.setRadius(1.5);
        //this->sprite = sprite;
        this->StartPosition = StartPosition;
        this->MousePosition = MousePosition;
        this->speed = speed;
        this->arrow.setPosition(StartPosition.GetXF(), StartPosition.GetYF());
        this->CurrentPosition = StartPosition;
        cout << "Position: " << arrow.getPosition().x << " " << arrow.getPosition().y;
        float path = sqrt(pow(StartPosition.GetYF() - MousePosition.GetYF(), 2) + pow(StartPosition.GetXF() - MousePosition.GetXF(), 2));
        sin = (StartPosition.GetYF() - MousePosition.GetYF()) / path;
        cos = (StartPosition.GetXF() - MousePosition.GetXF()) / path;

    }

    void Update(float deltaTime) {

        CurrentPosition.SetXF(CurrentPosition.GetXF() - speed * deltaTime * cos);
        CurrentPosition.SetYF(CurrentPosition.GetYF() - speed * deltaTime * sin);

        arrow.setPosition(CurrentPosition.GetXF(), CurrentPosition.GetYF());

        //if (arrow.getPosition().x < 0 || arrow.getPosition().x > WORLD_HEIGHT*TEXTURE_SIZE || arrow.getPosition().y < 0 || arrow.getPosition().y > WORLD_HEIGHT*TEXTURE_SIZE) {
            
        //}

    }

    CircleShape GetSprite() {
        return arrow;
    }

};

class GUI {
private:
    vector<Sprite> GUIObjects;

public:

    GUI(int CurrentRoad) {

        GUIObjects.push_back(RoadtypeBackground);
        GUIObjects[0].setPosition(WINDOW_WIDTH - 10, WINDOW_HEIGHT - 10);
        GUIObjects.push_back(RoadSprites[CurrentRoad]);
        GUIObjects[1].setPosition(WINDOW_WIDTH - 28, WINDOW_HEIGHT - 28);
        GUIObjects[1].scale(1.5, 1.5);

    }

    vector<Sprite> GetObjects() {
        return GUIObjects;
    }

    void UpdateRoad(int CurrentRoad) {
        GUIObjects[1].setTextureRect(RoadSprites[CurrentRoad].getTextureRect());
    }

};

// Точка входа
int main() {

    // Зерно рандома
    srand(static_cast<unsigned int>(time(0)));

    // Загрузка материалов игры
    LoadSource();

    // Создание карты игры
    vector<vector<Cell>> GameField = DefaultMap();

    // Создание персонажа
    Character character(CharacterSprite, Position2D(static_cast<float>(WORLD_WIDTH*TEXTURE_SIZE/2 - 32), static_cast<float>(WORLD_HEIGHT*TEXTURE_SIZE/2 - 32)));

    // Создание анимации персонажа
    Animation CharacterAnimation(character.GetSprite(), 0.07f, Vector2u(10,10));

    // Создание камеры персонажа
    View CharacterView(FloatRect((WORLD_WIDTH*TEXTURE_SIZE-VIEW_WIDTH)/2, (WORLD_HEIGHT*TEXTURE_SIZE-VIEW_HEIGHT)/2, VIEW_WIDTH, VIEW_HEIGHT));

    // Создание главного окна игры
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE, Style::Fullscreen);

    // Конфигурация главного окна игры
    //window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(61);
    window.setView(CharacterView);
    window.setIcon(512,512,icon.getPixelsPtr());

    // Нужно для сохранение выбранного типа дороги для генератора
    int current_road_type = 0;  

    // Инициализация переменной,хранящей время одного кадра
    float deltaTime = 0.0f;
    
    // Создание таймера
    Clock clock;

    // Инициализация переменной, хранящей направление персонажа
    bool LastFaceRight = true;

    // Запуск звука ветра на фоне
    WindSound.play();

    vector<ProjectTile> LiveProjectTiles(0);
    GUI gui(current_road_type);

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

                if (current_road_type + 1 == 32) 
                    current_road_type = 0;
                else 
                    current_road_type++;

                gui.UpdateRoad(current_road_type);
                
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
                        GameField[row][col].ChangeCell(RoadSprites[current_road_type], ROAD);     

                        // Воспроизведение звука строительства
                        BuildSound.play();

                    }

                } 

            }

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left){

                Position2D CurrentPosition(character.GetPosition().GetXF() + CHARACTER_SIZE * CHARACTER_SCALE_FACTOR / 2, character.GetPosition().GetYF() + CHARACTER_SIZE * CHARACTER_SCALE_FACTOR / 2);
                Position2D MousePosition((float)window.mapPixelToCoords(Mouse::getPosition(window)).x, (float)window.mapPixelToCoords(Mouse::getPosition(window)).y);
                LiveProjectTiles.push_back(ProjectTile(CurrentPosition, MousePosition, 600));

            }

        }

        // Переменная, хранящая информацию о том, менялась ли анимация в этой итерации цикла
        bool updated = false;

        // Движение в право
        if (Keyboard::isKeyPressed(Keyboard::D)) {
            
            // Проверка на выход за границы мира
            if (character.GetPosition().GetXF() + deltaTime*CHARACTER_SPEED + CHARACTER_SIZE * CHARACTER_SCALE_FACTOR - 1 <= WORLD_WIDTH*TEXTURE_SIZE) {

                // Движение персонажа
                character.Move(RIGHT, deltaTime);

                // Проверка на повторяемость анимации
                if (updated == false) { 
                    CharacterAnimation.Update(2, deltaTime, true); 
                    updated = true;
                }

                // Нужно для корректной работы камеры
                if (CharacterView.getCenter().x + VIEW_WIDTH/2 + deltaTime*CHARACTER_SPEED <= WORLD_WIDTH*TEXTURE_SIZE) {
                    if (!(character.GetPosition().GetXF() + CHARACTER_SIZE*CHARACTER_SCALE_FACTOR/2 < CharacterView.getCenter().x)) {
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
            if (character.GetPosition().GetYF() + deltaTime*CHARACTER_SPEED + CHARACTER_SIZE * CHARACTER_SCALE_FACTOR - 1 <= WORLD_HEIGHT*TEXTURE_SIZE) {

                // Движение персонажа
                character.Move(BOTTOM, deltaTime);

                // Проверка на повторяемость анимации
                if (updated == false) {
                    CharacterAnimation.Update(2, deltaTime, LastFaceRight);
                    updated = true;
                }

                // Нужно для корректной работы камеры
                if (CharacterView.getCenter().y + VIEW_HEIGHT/2 + deltaTime*CHARACTER_SPEED <= WORLD_HEIGHT*TEXTURE_SIZE) {
                    if (!(character.GetPosition().GetYF() + CHARACTER_SIZE*CHARACTER_SCALE_FACTOR/2 < CharacterView.getCenter().y)) {
                        CharacterView.move(0.f, deltaTime*CHARACTER_SPEED);
                        window.setView(CharacterView);
                    }
                }

                // Сообщить, что персонаж двигается
                isMoving = true;
            }
        } 

        if (Keyboard::isKeyPressed(Keyboard::A)) {

            // Проверка на выход за границы мира
            if (character.GetPosition().GetXF() - deltaTime*CHARACTER_SPEED + 1 >= 0) {

                // Движение персонажа
                character.Move(LEFT, deltaTime);

                // Проверка на повторяемость анимации
                if (updated == false) {
                    CharacterAnimation.Update(2, deltaTime, false);
                    updated = true;
                }

                // Нужно для корректной работы камеры
                if (CharacterView.getCenter().x - VIEW_WIDTH/2 - deltaTime*CHARACTER_SPEED >= 0) {

                    if (!(character.GetPosition().GetXF() + CHARACTER_SIZE*CHARACTER_SCALE_FACTOR/2 > CharacterView.getCenter().x)) {
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
            if (character.GetPosition().GetYF() - deltaTime*CHARACTER_SPEED >= 0) {

                // Движение персонажа
                character.Move(TOP, deltaTime);

                // Проверка на повторяемость анимации
                if (updated == false) {
                    CharacterAnimation.Update(2, deltaTime, LastFaceRight);
                    updated = true;
                }

                // Нужно для корректной работы камеры
                if (CharacterView.getCenter().y - VIEW_HEIGHT/2 - deltaTime*CHARACTER_SPEED >= 0) {
                    if (!(character.GetPosition().GetYF() + CHARACTER_SIZE*CHARACTER_SCALE_FACTOR/2 > CharacterView.getCenter().y)) {
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

        // Обновление спрайта персонажа и его отрисовка
        character.GetSpriteRef()->setTextureRect(CharacterAnimation.GetIntRect());
        window.draw(character.GetSprite());

        for (int i = 0; i < LiveProjectTiles.size(); i++) {
            LiveProjectTiles[i].Update(deltaTime);
            window.draw(LiveProjectTiles[i].GetSprite());
        }

        vector<Sprite> GUIObjects = gui.GetObjects();
        for (int i = 0; i < GUIObjects.size(); i++) {

            Sprite temp = GUIObjects[i];
            double x, y;
            x = temp.getPosition().x / WINDOW_WIDTH;
            y = temp.getPosition().y / WINDOW_HEIGHT;
            temp.setPosition(CharacterView.getCenter().x - VIEW_WIDTH / 2 + VIEW_WIDTH * x - temp.getGlobalBounds().width, CharacterView.getCenter().y - VIEW_HEIGHT / 2 + VIEW_HEIGHT * y - temp.getGlobalBounds().height);
            window.draw(temp);

        }

        // Отрисовка окна
        window.display();

    }

}
