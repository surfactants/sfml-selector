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
            sf::Color n_selected = sf::Color(150, 130, 190),
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
    Option(std::string n_data, sf::Font& font, sf::Vector2f size, sf::Vector2f pos, Selector_Palette* n_palette);

    ////////////////////////////////////////////////////
    /// \brief Changes stored value
    ///
    /// \param \b n_data -> text
    ///
    void setData(std::string n_data);

    ////////////////////////////////////////////////////
    /// \brief Re-sets colors after palette change
    ///
    /// \param \b n_palette -> palette
    ///
    void refresh();

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
    /// \brief Sets colors to indicate mouseover
    ///
    void highlight();

    ////////////////////////////////////////////////////
    /// \brief Sets colors to default
    ///
    void unhighlight();

    ////////////////////////////////////////////////////
    /// \brief Returns selection state
    ///
    /// \return \b bool selected
    ///
    bool isSelected();

    ////////////////////////////////////////////////////
    /// \brief Sets colors to indicate selection
    ///
    void select();

    ////////////////////////////////////////////////////
    /// \brief Reverts colors to highlight state
    ///
    void unselect();

    ///Value stored in the option.
    std::string data;

    ///Option container
    sf::RectangleShape box;

    ///Option text (from data)
    sf::Text text;

    ///Color palette
    Selector_Palette* palette;

private:
    ///Tracks mouseover state
    bool highlighted{ false };

    ///Tracks click state
    bool selected{ false };

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
    /// \brief Sets Option box size
    ///
    /// \param \b n_size -> option_size
    ///
    void setOptionSize(sf::Vector2f n_size);

    ///////////////////////////////////////////////
    /// \brief Changes font size
    ///
    /// \param \b f_size new font size
    /// \param \b dynamic_resize whether to reset option_size based on f_size (defaults to true)
    ///
    void setFontSize(unsigned int f_size, bool dynamic_resize = true);

    ///////////////////////////////////////////////
    /// \brief Sets render distance (options displayed - 1)
    ///
    /// \param \b ndistance if this value is too large, the distance will be set by options.size
    ///
    void setRenderDistance(unsigned int ndistance);

    ///////////////////////////////////////////////
    /// \brief Reconstructs the palette and passes it to the options
    ///
    /// \param 4 palette colors (light, dark, selected, selected_text)
    ///
    void setColors(sf::Color n_light, sf::Color n_dark, sf::Color n_selected, sf::Color n_selected_text);

    ///////////////////////////////////////////////
    /// \brief Returns the selected option value
    ///
    std::string getSelection();

    ///////////////////////////////////////////////
    /// \brief Checks and sets highlight states
    ///
    /// \param \b mpos mouse position
    ///
    void checkMouse(sf::Vector2f mpos);

    ///////////////////////////////////////////////
    /// \brief Parses left click
    ///
    void clickLeft();

    ///////////////////////////////////////////////
    /// \brief Moves options if the selector is open and moused, and if the movement is in bounds
    ///
    /// \param \b delta event.mouseWheelScroll.delta
    ///
    void scroll(float delta);

private:
    ///Holds the open menu and scrollbar
    sf::RectangleShape frame;

    ///Options in dropdown menu
    std::vector<Option> options;

    ///Option displayed when closed
    Option selected;

    ///Option box size
    sf::Vector2f option_size{ sf::Vector2f(192.f, 48.f) };

    ///Currently selected option (for unselecting)
    unsigned int select_index{ 0 };

    ///First option to render when open
    unsigned int render_start{ 0 };

    ///Number of options to render when open (INDEXED, ACTUAL COUNT IS THIS + 1)
    unsigned int render_distance{ 4 };

    ///Mouseover
    bool moused{ false };

    ///Is the menu open?
    bool open{ false };

    ///Tracks scroll bounds
    sf::RectangleShape scrollbar;

    ///... Scrollbar x size
    float scrollbar_size_x{ 16.f };

    ///Option color palette
    Selector_Palette palette;

    ////////////////////////////////////////////////////
    /// \brief Opens the drop-down
    /// Internal
    ///
    void setOpen();

    ////////////////////////////////////////////////////
    /// \brief Closes the drop-down
    /// Internal
    ///
    void setClosed();

    ////////////////////////////////////////////////////
    /// \brief Resizes the scrollbar
    /// Internal
    ///
    void setScrollbar();

    ////////////////////////////////////////////////////
    /// \brief Repositions the scrollbar
    /// Internal
    ///
    void placeScrollbar();

    ////////////////////////////////////////////////////
    /// \brief Moves render_start to selected option (or as close as possible)
    /// Internal
    ///
    void resetRenderIndices();

    ////////////////////////////////////////////////////
    /// \brief Sets option_size based on font size
    /// Internal
    ///
    void calculateOptionSize();

    ////////////////////////////////////////////////////
    /// \brief Sets option positions based on their size
    /// Internal
    ///
    void placeOptions();

    ////////////////////////////////////////////////////
    /// \brief Draws elements to the render target
    /// Internal
    ///
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
