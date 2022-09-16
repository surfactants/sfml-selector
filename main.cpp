#include "selector.hpp"
#include <string>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(), "selector", sf::Style::Fullscreen);

    sf::Font font;
    font.loadFromFile("adventpro-medium.ttf");

    std::vector<std::string> s{ "lorem", "ipsum", "test", "whatever", "who cares", "69420", "hehe", "test2" };

    Selector selector(font, s);

    selector.setPosition(sf::Vector2f(48.f, 48.f));

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
                    selector.clickLeft();
                }
            }
            else if(event.type == sf::Event::MouseWheelScrolled){
                std::cout << event.mouseWheelScroll.delta << '\n';
                selector.scroll(event.mouseWheelScroll.delta);
            }
        }

        selector.checkMouse(sf::Vector2f(sf::Mouse::getPosition()));

        window.clear();
            window.draw(selector);
        window.display();
    }

    return 0;
}
