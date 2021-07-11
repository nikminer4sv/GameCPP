#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

int main() {

    RenderWindow window(VideoMode(800,600), "Title");

    while(window.isOpen()) {

        Event event;

        while(window.pollEvent(event)) {

            if (event.type == Event::Closed) {
                window.close();
            }

        }

        window.clear(Color::White);

        RectangleShape rect(Vector2f(200,100));
        rect.setFillColor(Color::Red);
        rect.setPosition(300,250);
        window.draw(rect);

        window.display();

    }

    return 0;

}