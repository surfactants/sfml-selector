#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include <type_traits>

struct Option : public sf::Drawable{
    Option() = default;

    Option(std::string t, sf::Font& font, sf::Vector2f size, sf::Vector2f pos);

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
    sf::Color color_dark{ sf::Color(50, 70, 60) };
    sf::Color color_selected{ sf::Color(160, 170, 210) };

    void highlight();
    void unhighlight();

    void select();
    void unselect();

    void setPosition(sf::Vector2f pos);
    void move(int factor);

    void refresh(); //for tracking color changes
};

class Selector : public sf::Drawable{
public:
    Selector(sf::Font& font, std::vector<std::string> ts, sf::Vector2f pos);

    void checkMouse(sf::Vector2f mpos);

    std::string getSelection();

    void clickLeft();

    void scroll(float delta);

    void setColors(sf::Color nlight, sf::Color ndark, sf::Color nselect);

private:
    std::vector<Option> options;

    sf::Vector2f option_size{ sf::Vector2f(128.f, 48.f) };

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
