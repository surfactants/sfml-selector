#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>


////////////////////////////////////////////////////
/// \brief Holds color information for Option states
///
struct Selector_Palette{
    Selector_Palette(sf::Color n_light = sf::Color(210, 190, 185),
            sf::Color n_dark = sf::Color(50, 70, 60),
            sf::Color n_selected = sf::Color(160, 170, 210),
            sf::Color n_selected_text = sf::Color(240, 240, 240))
        : color_light{ n_light }, color_dark{ n_dark }, color_selected{ n_selected }, color_selected_text{ n_selected_text }{}

    sf::Color color_light;
    sf::Color color_dark;
    sf::Color color_selected;
    sf::Color color_selected_text;
};

////////////////////////////////////////////////////
/// \brief Defines an individual option for Selector class
///
class Option : public sf::Drawable{
public:
    Option() = default;

    ////////////////////////////////////////////////////
    /// \brief Primary constructor
    ///
    /// \param \b n_data -> text
    /// \param \b font -> text
    /// \param \b size -> box
    /// \param \b pos -> box, text
    ///
    Option(std::string n_data, sf::Font& font, sf::Vector2f size, sf::Vector2f pos);

    ////////////////////////////////////////////////////
    /// \brief Changes stored value
    ///
    /// \param \b n_data -> text
    ///
    void setData(std::string n_data);

    ////////////////////////////////////////////////////
    /// \brief Sets highlight state according to mouse position
    ///
    /// \param \b mpos Mouse position
    ///
    void checkMouse(sf::Vector2f mpos);

    ////////////////////////////////////////////////////
    /// \brief Returns mouseover state
    ///
    /// \return \b bool highlighted
    ///
    bool isHighlighted();

    ////////////////////////////////////////////////////
    /// \brief Returns selection state
    ///
    /// \return \b bool selected
    ///
    bool isSelected();

    ////////////////////////////////////////////////////
    /// \brief Sets colors to indicate mouseover
    ///
    void highlight();

    ////////////////////////////////////////////////////
    /// \brief Sets colors to default
    ///
    void unhighlight();

    ////////////////////////////////////////////////////
    /// \brief Sets colors to indicate selection
    ///
    void select();

    ////////////////////////////////////////////////////
    /// \brief Reverts colors to highlight state
    ///
    void unselect();

    ////////////////////////////////////////////////////
    /// \brief Places elements
    ///
    /// \param \b pos new element position
    ///
    void setPosition(sf::Vector2f pos);

    ////////////////////////////////////////////////////
    /// \brief Moves by a calculated value
    ///
    /// \param \b factor multiplied by box.size.y to find displacement
    ///
    void move(int factor);

    ////////////////////////////////////////////////////
    /// \brief Moves by a fixed value
    ///
    /// \param \b distance displacement value
    ///
    void move(float distance);

    ///Value stored in the option.
    std::string data;

    ///Option container
    sf::RectangleShape box;

    ///Option text (from data)
    sf::Text text;

    ///Color palette
    Selector_Palette palette;

    ////////////////////////////////////////////////////
    /// \brief Changes the color palette
    ///
    /// \param \b n_palette -> palette
    ///
    void changePalette(Selector_Palette n_palette);

private:
    ///Tracks mouseover state
    bool highlighted{ false };

    ///Tracks click state
    bool selected{ false };

    ////////////////////////////////////////////////////
    /// \brief Resets for color changes
    ///
    void refresh();

    ////////////////////////////////////////////////////
    /// \brief Draws elements to the render target
    ///
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

////////////////////////////////////////////////////
/// \brief Dropdown selector. Option values are stored as strings.
///
class Selector : public sf::Drawable{
public:
    ////////////////////////////////////////////////////
    /// \brief Primary constructor
    ///
    /// \param \b font reference to a font object
    /// \param \b data vector containing option values
    /// \param \b pos dropdown location
    ///
    Selector(sf::Font& font, std::vector<std::string> data, sf::Vector2f pos);

    ///////////////////////////////////////////////
    /// \brief
    ///
    void checkMouse(sf::Vector2f mpos);

    ///////////////////////////////////////////////
    /// \brief
    ///
    std::string getSelection();

    ///////////////////////////////////////////////
    /// \brief
    ///
    void clickLeft();

    ///////////////////////////////////////////////
    /// \brief
    ///
    void scroll(float delta);

    ///////////////////////////////////////////////
    /// \brief Reconstructs the palette and passes it to the options
    ///
    void setColors(sf::Color n_light, sf::Color n_dark, sf::Color n_selected, sf::Color n_selected_text);

    ///////////////////////////////////////////////
    /// \brief Sets render distance (options displayed - 1)
    /// If this value is too large, the distance will be set by options.size
    ///
    void setRenderDistance(unsigned int ndistance);

    ///////////////////////////////////////////////
    /// \brief
    ///
    void setFontSize(unsigned int f_size, bool dynamic_resize = true);

    ///////////////////////////////////////////////
    /// \brief
    ///
    void setOptionSize(sf::Vector2f nsize);

private:
    std::vector<Option> options;

    Selector_Palette palette;

    sf::Vector2f option_size{ sf::Vector2f(192.f, 48.f) };

    bool moused{ false };

    Option selected;
    unsigned int select_index{ 0 };

    bool open{ false };

    sf::RectangleShape frame;

    unsigned int render_start{ 0 };
    unsigned int render_distance{ 4 };

    void setOpen();

    void setClosed();

    void resetRenderIndices();

    sf::RectangleShape scrollbar;
    float scrollbar_size_x{ 16.f };

    void setScrollbar();
    void placeScrollbar();

    void calculateOptionSize();

    void placeOptions();

    Selector_Palette palette;

    ////////////////////////////////////////////////////
    /// \brief Draws elements to the render target
    ///
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
