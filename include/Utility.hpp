#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <sstream>
#include <cmath>

#define PI 3.14159265f

template <typename T>
std::string toString(const T& value);

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);
int randomInt(int exclusiveMax);
float length(sf::Vector2f vector);
sf::Vector2f unitVector(sf::Vector2f vector);
float unitVectorAngle(const sf::Vector2f& vector);
float vectorAngle(const sf::Vector2f& vector);

template <typename T>
inline std::string toString(const T& value)
{
    std::stringstream stream;
    stream << value;
    return stream.str();
}