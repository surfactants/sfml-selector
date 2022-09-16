#pragma once

////////////////////////////////////////////////////////////
/// \brief Seeds the random number generator.
/// To be run once, at the beginning of int main()
///
/// \see randomInteger(), randomFloat(), randomBool(), weightedBool()
///
////////////////////////////////////////////////////////////
inline void randomSeed()
{
    srand(time(NULL));
}

////////////////////////////////////////////////////////////
/// \brief Returns a random whole number between floor and ceil.
///
/// \param \b floor the lowest possible result
/// \param \b ceil the highest possible result
///
/// \return random
///
/// \see randomFloat(), randomBool(), weightedBool()
///
////////////////////////////////////////////////////////////
inline int randomInteger(int floor, int ceil)
{
    ++ ceil -= floor;
    int random = rand()%ceil + floor;
    return random;
}

////////////////////////////////////////////////////////////
/// \brief Returns a random color, transparency alpha.
///
/// \param \b alpha transparency
///
/// \return \b color
///
////////////////////////////////////////////////////////////
inline sf::Color randomColor(unsigned int alpha = 255)
{
    unsigned int r = randomInteger(0,255);
    unsigned int g = randomInteger(0,255);
    unsigned int b = randomInteger(0,255);
    return sf::Color(r,g,b,alpha);
}
