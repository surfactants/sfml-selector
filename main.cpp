#include "selector.hpp"
#include <string>
#include <iostream>
#include "randomcolor.hpp"

int main()
{
    randomSeed();

    sf::RenderWindow window(sf::VideoMode(), "selector", sf::Style::Fullscreen);

    sf::Font font;
    font.loadFromFile("adventpro-medium.ttf");

    std::vector<std::string> s{ "lorem", "ipsum", "test", "whatever", "who cares", "69420", "hehe", "test2" };

    Selector selector(font, s, sf::Vector2f(48.f, 48.f));

    sf::RectangleShape button(sf::Vector2f(64.f, 64.f));
    button.setFillColor(sf::Color::Red);
    button.setPosition(1600.f, 16.f);

    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }
            else if(event.type == sf::Event::KeyReleased){
                if(event.key.code == sf::Keyboard::Escape){
                    window.close();
                }
                else if(event.key.code == sf::Keyboard::Up){
                }
                else if(event.key.code == sf::Keyboard::Down){
                }
            }
            else if(event.type == sf::Event::MouseButtonPressed){
                if(event.mouseButton.button == sf::Mouse::Left){
                    if(button.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition()))){
                        selector.setColors(randomColor(), randomColor(), randomColor());
                    }
                    else selector.clickLeft();
                }
            }
            else if(event.type == sf::Event::MouseWheelScrolled){
                selector.scroll(event.mouseWheelScroll.delta);
            }
        }

        selector.checkMouse(sf::Vector2f(sf::Mouse::getPosition()));

        window.clear();
            window.draw(selector);
            window.draw(button);
        window.display();
    }

    return 0;
}
