#include "selector.hpp"
#include <iostream>

Option::Option(std::string t, sf::Font& font)
{
    text.setFont(font);
    setData(t);

    box.setSize(sf::Vector2f(96.f, 96.f));

    unhighlight();
}

void Option::setData(std::string t)
{
    data = t;
    text.setString(data);
}

void Option::checkMouse(sf::Vector2f mpos)
{
    bool contains = box.getGlobalBounds().contains(mpos);
    if(highlighted && !contains){
        unhighlight();
    }
    else if(!highlighted && contains){
        highlight();
    }
}

void Option::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(box, states);
    target.draw(text, states);
}

bool Option::isHighlighted()
{
    return highlighted;
}

bool Option::isSelected()
{
    return selected;
}

void Option::highlight()
{
    highlighted = true;
    box.setFillColor(color_light);
    text.setFillColor(color_dark);
}

void Option::unhighlight()
{
    highlighted = false;
    if(selected) select();
    else{
        box.setFillColor(color_dark);
        text.setFillColor(color_light);
    }
}

void Option::select()
{
    selected = true;
    box.setFillColor(sf::Color::Blue);
    text.setFillColor(color_dark);
}

void Option::unselect()
{
    selected = false;
    if(highlighted) highlight();
    else unhighlight();
}

void Option::setPosition(sf::Vector2f pos)
{
    box.setPosition(pos);
    text.setPosition(pos);
}

void Option::move(int factor)
{
    box.move(sf::Vector2f(0.f, box.getSize().y * factor));
    text.move(sf::Vector2f(0.f, box.getSize().y * factor));
}

Selector::Selector(sf::Font& font, std::vector<std::string> ts)
{
    for(const auto& t : ts){
        options.push_back(Option(t, font));
    }

    options.front().select();
    selected = options.front();
}

void Selector::checkMouse(sf::Vector2f mpos)
{
    if(open){
        moused = frame.getGlobalBounds().contains(mpos);
        for(auto& o : options){
            o.checkMouse(mpos);
        }
    }
    else{
        moused = selected.box.getGlobalBounds().contains(mpos);
        selected.checkMouse(mpos);
    }
}

std::string Selector::getSelection()
{
    return selected.data;
}

void Selector::clickLeft()
{
    if(open){
        for(unsigned int i = render_start; i <= render_end; ++i){
            if(options[i].isHighlighted() && !options[i].isSelected()){
                std::cout << "changing selection from " << select_index << " to " << i;
                options[select_index].unselect();
                options[i].select();
                select_index = i;
                selected.setData(options[i].data);
                break;
            }
        }

        setClosed();
    }
    else{
        if(moused){
            resetRenderIndices();
            setOpen();
        }
    }
}

void Selector::resetRenderIndices()
{
    int factor = 0;
    while(select_index < render_start){
        render_start--;
        render_end--;
        factor++;
    }

    while(select_index > render_start){
        if(render_end == options.size() - 1) break;
        render_start++;
        render_end++;
        factor--;
    }

    for(auto& o : options){
        o.move(factor);
    }
    if(select_index < render_start){
        while(select_index < render_start){
        }
    }
}

void Selector::setPosition(sf::Vector2f pos)
{
    selected.setPosition(pos);

    for(auto& o : options){
        o.setPosition(pos);
        //pos.y += option_size.y;
        pos.y += 96.f;
    }
}

void Selector::setOpen()
{
    open = true;
}

void Selector::setClosed()
{
    open = false;
}

void Selector::scroll(float delta)
{
    if(open){
        std::cout << "scrolling! " << delta << '\n';
        bool upward = (delta > 0.f);
        int factor = 1;
        if(upward){
            std::cout << "\tupward\n";
            if(render_start > 0){
                std::cout << "\tcan\n";
                render_start--;
                render_end--;
            }
            else return;
        }
        else{
            std::cout << "\tdownward\n";
            if(render_end < options.size() - 1){
                std::cout << "\tcan\n";
                render_start++;
                render_end++;
                factor = -1;
            }
            else return;
        }

        for(auto& o : options)
        {
            o.move(factor);
        }
    }
}

void Selector::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(!open){
        target.draw(selected, states);
    }
    else{
        target.draw(frame, states);
        for(unsigned int i = render_start; i <= render_end; ++i){
            target.draw(options[i], states);
        }
    }
}
