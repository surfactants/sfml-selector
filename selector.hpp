#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include <type_traits>

struct Option : public sf::Drawable{
    Option() = default;

    Option(std::string t, sf::Font& font);

    std::string data;

    sf::RectangleShape box;
    sf::Text text;

    void setData(std::string t);

    void checkMouse(sf::Vector2f mpos);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    bool highlighted{ false };
    bool selected{ false };

    bool isHighlighted();
    bool isSelected();

    sf::Color color_light{ sf::Color(210, 190, 185) };
    sf::Color color_dark{ sf::Color(40, 70, 60) };

    void highlight();
    void unhighlight();

    void select();
    void unselect();

    void setPosition(sf::Vector2f pos);
    void move(int factor);
};

class Selector : public sf::Drawable{
public:
    Selector(sf::Font& font, std::vector<std::string> ts);

    void checkMouse(sf::Vector2f mpos);

    std::string getSelection();

    void clickLeft();

    void setPosition(sf::Vector2f pos);

    void scroll(float delta);

private:
    std::vector<Option> options;

    bool moused{ false };

    Option selected;
    unsigned int select_index{ 0 };

    bool open{ false };

    sf::RectangleShape frame;

    unsigned int render_start{ 0 };
    unsigned int render_end{ 4 };

    void setOpen();

    void setClosed();

    void resetRenderIndices();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
