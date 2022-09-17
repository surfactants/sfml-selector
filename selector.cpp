#include "selector.hpp"

Option::Option(std::string n_data, sf::Font& font, sf::Vector2f size, sf::Vector2f pos)
{
    text.setFont(font);
    setData(n_data);

    box.setSize(size);

    setPosition(pos);

    unhighlight();
}

void Option::setData(std::string n_data)
{
    data = n_data;
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
    box.setFillColor(palette.color_light);
    text.setFillColor(palette.color_dark);
}

void Option::unhighlight()
{
    highlighted = false;
    if(selected) select();
    else{
        box.setFillColor(palette.color_dark);
        text.setFillColor(palette.color_light);
    }
}

void Option::select()
{
    selected = true;
    box.setFillColor(palette.color_selected);
    text.setFillColor(palette.color_selected_text);
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

void Option::move(float distance)
{
    box.move(sf::Vector2f(0.f, distance));
    text.move(sf::Vector2f(0.f, distance));
}

void Option::refresh()
{
    if(highlighted) highlight();
    else{
        if(selected) select();
        else unhighlight();
    }
}

void Option::changePalette(Selector_Palette n_palette)
{
    palette = n_palette;
    refresh();
}

Selector::Selector(sf::Font& font, std::vector<std::string> data, sf::Vector2f pos)
{
    selected.setPosition(pos);

    frame.setPosition(pos);
    frame.setFillColor(sf::Color(69, 69, 69));

    setOptionSize(option_size);

    for(const auto& d : data){
        options.push_back(Option(d, font, option_size, pos));
        options.back().setPosition(pos);
        pos.y += option_size.y;
    }

    options.front().select();
    selected = options.front();
    scrollbar.setFillColor(sf::Color(169, 169, 169));


    setRenderDistance(render_distance);

    setScrollbar();
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
        if(moused){
            for(unsigned int i = render_start; i <= render_start + render_distance; ++i){
                if(options[i].isHighlighted() && !options[i].isSelected()){
                    options[select_index].unselect();
                    options[i].select();
                    select_index = i;
                    selected.setData(options[i].data);
                    break;
                }
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
        factor++;
    }

    while(select_index > render_start){
        if(render_start + render_distance == options.size() - 1) break;
        render_start++;
        factor--;
    }

    for(auto& o : options){
        o.move(factor);
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
        bool upward = (delta > 0.f);
        int factor = 1;
        if(upward){
            if(render_start > 0){
                render_start--;
                placeScrollbar();
            }
            else return;
        }
        else{
            if(render_start + render_distance < options.size() - 1){
                render_start++;
                placeScrollbar();
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
        for(unsigned int i = render_start; i <= render_start + render_distance; ++i){
            target.draw(options[i], states);
        }
        target.draw(scrollbar, states);
    }
}

void Selector::setColors(sf::Color n_light, sf::Color n_dark, sf::Color n_selected, sf::Color n_selected_text)
{
    palette = Selector_Palette(n_light, n_dark, n_selected, n_selected_text);

    selected.changePalette(palette);

    for(auto& o : options){
        o.changePalette(palette);
    }
}

void Selector::setRenderDistance(unsigned int ndistance)
{
    if(ndistance >= options.size())
    {
        render_distance = options.size() - 1;
        render_start = 0;
    }
    else{
        render_distance = ndistance;

        while(render_start + render_distance >= options.size() - 1)
        {
            render_start--;
        }
    }

    frame.setSize(sf::Vector2f(option_size.x + scrollbar_size_x, option_size.y * (render_distance + 1)));

    setScrollbar();
}

void Selector::setScrollbar()
{
    float size_y = ((static_cast<float>(render_distance + 1)) / (options.size() - 1));
    size_y *= frame.getSize().y;

    scrollbar.setSize(sf::Vector2f(scrollbar_size_x, size_y));

    placeScrollbar();
}

void Selector::placeScrollbar()
{
    float index = static_cast<float>(render_start) / (options.size() - 1 - render_distance);

    float pos_y = frame.getPosition().y + (index * (frame.getSize().y - scrollbar.getSize().y));

    scrollbar.setPosition(sf::Vector2f(frame.getPosition().x + frame.getSize().x - scrollbar_size_x, pos_y));
}

void Selector::setFontSize(unsigned int f_size, bool dynamic_resize)
{
    selected.text.setCharacterSize(f_size);

    if(dynamic_resize)
    {
        calculateOptionSize();
    }

    for(auto& o : options){
        o.text.setCharacterSize(f_size);
    }
}

void Selector::calculateOptionSize()
{
    float size = selected.text.getCharacterSize();
    float x = 64.f + (size * 3.f);
    float y = size * 1.5f;
    setOptionSize(sf::Vector2f(x, y));
}

void Selector::setOptionSize(sf::Vector2f nsize)
{
    frame.setSize(sf::Vector2f(nsize.x + scrollbar_size_x, nsize.y * (render_distance + 1)));

    selected.box.setSize(nsize);

    for(auto& o : options){
        o.box.setSize(nsize);
    }

    option_size = nsize;

    placeOptions();

    setScrollbar();
}

void Selector::placeOptions()
{
    sf::Vector2f pos = selected.box.getPosition();

    for(auto& o : options){
        o.setPosition(pos);
        pos.y += option_size.y;
        o.move((int)render_start);

    }
}
