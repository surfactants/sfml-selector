#include "selector.hpp"

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
Option::Option(std::string n_data, sf::Font& font, sf::Vector2f size, sf::Vector2f pos, Selector_Palette* n_palette)
    : palette{ n_palette }
{
    text.setFont(font);
    setData(n_data);

    box.setSize(size);

    setPosition(pos);

    unhighlight();
}

///////////////////////////////////////////////////////////////////////////////////////////
void Option::setData(std::string n_data)
{
    data = n_data;
    text.setString(data);
}

///////////////////////////////////////////////////////////////////////////////////////////
void Option::refresh()
{
    if(highlighted) highlight();
    else{
        if(selected) select();
        else unhighlight();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
void Option::setPosition(sf::Vector2f pos)
{
    box.setPosition(pos);
    text.setPosition(pos);
}

///////////////////////////////////////////////////////////////////////////////////////////
void Option::move(int factor)
{
    box.move(sf::Vector2f(0.f, box.getSize().y * factor));
    text.move(sf::Vector2f(0.f, box.getSize().y * factor));
}

///////////////////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////////////////
bool Option::isHighlighted()
{
    return highlighted;
}

///////////////////////////////////////////////////////////////////////////////////////////
void Option::highlight()
{
    highlighted = true;
    box.setFillColor(palette->color_light);
    text.setFillColor(palette->color_dark);
}

///////////////////////////////////////////////////////////////////////////////////////////
void Option::unhighlight()
{
    highlighted = false;
    if(selected) select();
    else{
        box.setFillColor(palette->color_dark);
        text.setFillColor(palette->color_light);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
bool Option::isSelected()
{
    return selected;
}

///////////////////////////////////////////////////////////////////////////////////////////
void Option::select()
{
    selected = true;
    box.setFillColor(palette->color_selected);
    text.setFillColor(palette->color_selected_text);
}

///////////////////////////////////////////////////////////////////////////////////////////
void Option::unselect()
{
    selected = false;
    if(highlighted) highlight();
    else unhighlight();
}

///////////////////////////////////////////////////////////////////////////////////////////
void Option::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(box, states);
    target.draw(text, states);
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
Selector::Selector(sf::Font& font, std::vector<std::string> data, sf::Vector2f pos)
{
    selected.setPosition(pos);

    frame.setPosition(pos);
    frame.setFillColor(sf::Color(69, 69, 69));

    setOptionSize(option_size);

    for(const auto& d : data){
        options.push_back(Option(d, font, option_size, pos, &palette));
    }

    options.front().select();
    selected = options.front();

    scrollbar.setFillColor(sf::Color(169, 169, 169));

    setRenderDistance(render_distance);

    placeOptions();

    setScrollbar();
}

///////////////////////////////////////////////////////////////////////////////////////////
void Selector::setOptionSize(sf::Vector2f n_size)
{
    frame.setSize(sf::Vector2f(n_size.x + scrollbar_size_x, n_size.y * (render_distance + 1)));

    selected.box.setSize(n_size);

    for(auto& o : options){
        o.box.setSize(n_size);
    }

    option_size = n_size;

    placeOptions();

    setScrollbar();
}

///////////////////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////////////////
void Selector::setColors(sf::Color n_light, sf::Color n_dark, sf::Color n_selected, sf::Color n_selected_text)
{
    palette = Selector_Palette(n_light, n_dark, n_selected, n_selected_text);

    selected.refresh();

    for(auto& o : options){
        o.refresh();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
std::string Selector::getSelection()
{
    return selected.data;
}

///////////////////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////////////////
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
            setOpen();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
void Selector::scroll(float delta)
{
    if(open && moused){
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

///////////////////////////////////////////////////////////////////////////////////////////
void Selector::setOpen()
{
    open = true;
    resetRenderIndices();
    placeScrollbar();
}

///////////////////////////////////////////////////////////////////////////////////////////
void Selector::setClosed()
{
    open = false;
}

///////////////////////////////////////////////////////////////////////////////////////////
void Selector::setScrollbar()
{
    float size_y = ((static_cast<float>(render_distance + 1)) / (options.size() - 1));
    size_y *= frame.getSize().y;

    scrollbar.setSize(sf::Vector2f(scrollbar_size_x, size_y));

    placeScrollbar();
}

///////////////////////////////////////////////////////////////////////////////////////////
void Selector::placeScrollbar()
{
    float index = static_cast<float>(render_start) / (options.size() - 1 - render_distance);

    float pos_y = frame.getPosition().y + (index * (frame.getSize().y - scrollbar.getSize().y));

    scrollbar.setPosition(sf::Vector2f(frame.getPosition().x + frame.getSize().x - scrollbar_size_x, pos_y));
}

///////////////////////////////////////////////////////////////////////////////////////////
void Selector::resetRenderIndices()
{
    int factor = 0;

    factor = render_start - select_index;
    render_start = select_index;

    int threshold = options.size() - render_distance - 1;
    if(render_start >= threshold){
        int diff = threshold - render_start;
        render_start = threshold;
        factor -= diff;
    }

    for(auto& o : options){
        o.move(factor);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
void Selector::calculateOptionSize()
{
    float size = selected.text.getCharacterSize();
    const static float base = 64.f;
    const static sf::Vector2f factor(3.f, 1.5f);
    float x = base + (size * factor.x);
    float y = size * factor.y;
    setOptionSize(sf::Vector2f(x, y));
}

///////////////////////////////////////////////////////////////////////////////////////////
void Selector::placeOptions()
{
    sf::Vector2f pos = selected.box.getPosition();

    for(auto& o : options){
        o.setPosition(pos);
        pos.y += option_size.y;
        o.move((int)render_start);

    }
}

///////////////////////////////////////////////////////////////////////////////////////////
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
