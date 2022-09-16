#include "selector.hpp"
#include <string>

int main()
{
    sf::RenderWindow window(sf::VideoMode(), "selector", sf::Style::Fullscreen);
    sf::Event event;

    sf::Font font;
    font.loadFromFile("adventpro-medium.ttf");

    std::vector<int> i{ -642, 234, 12, 0, -3 };

    Selector<int> i_selector(font, i);

    std::vector<std::string> s{ "lorem", "ipsum", "test", "whatever", "who cares" };

    Selector<std::string> s_selector(font, s);

    while(window.isOpen()){
        while(window.pollEvent(event)){
            if(event.type == sf::Event::MouseButtonPressed){

            }
        }

        i_selector.checkMouse(sf::Vector2f(sf::Mouse::getPosition()));
        s_selector.checkMouse(sf::Vector2f(sf::Mouse::getPosition()));

        window.clear();
            window.draw(i_selector);
            window.draw(s_selector);
        window.display();
    }

    return 0;
}
