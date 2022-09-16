#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

template<typename T>
struct Option : public sf::Drawable{
    Option() = default;

    Option(T t, sf::Font& font)
    : data{ t }
    {
        text.setFont(font);
    }

    T data;

    sf::RectangleShape box;
    sf::Text text;

    void setData(T t)
    {
        data = t;
        text.setString(data);
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(box, states);
        target.draw(text, states);
    }

    void checkMouse(sf::Vector2f mpos)
    {
    }
};

template<typename T>
class Selector : public sf::Drawable{
public:
    Selector(sf::Font& font, std::vector<T> ts)
    {
        for(const auto& t : ts){
            options.push_back(Option(t, font));
        }
    }

    void checkMouse(sf::Vector2f mpos)
    {
        if(open)
        {
            if(frame.getGlobalBounds().contains(mpos)){
                for(auto& o : options){
                    o.checkMouse(mpos);
                }
            }
        }
    }

    T getSelection()
    {
        return selected.data;
    }

private:
    std::vector<Option<T>> options;

    Option<T> selected;

    bool open{ false };

    sf::RectangleShape frame;

    unsigned int render_start{ 0 };
    unsigned int render_end{ 0 };
    unsigned int render_distance{ 5 };

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if(open){
            target.draw(selected, states);
        }
        else{
            target.draw(frame, states);
            for(unsigned int i = render_start; i <= render_end; ++i){
                target.draw(options[i], states);
            }
        }
    }
};
