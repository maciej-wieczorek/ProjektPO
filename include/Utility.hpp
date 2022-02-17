#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <sstream>
#include <cmath>

#define PI 3.141592653589793238462643383f

template <typename T>
std::string toString(const T& value);

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);
int randomInt(int exclusiveMax);
float randomReal(float exclusiveMax);
int randomIntSigned(int exclusiveMax);
float length(sf::Vector2f vector);
sf::Vector2f unitVector(sf::Vector2f vector);
float unitVectorAngle(const sf::Vector2f& vector);
float vectorAngle(const sf::Vector2f& vector, float angle);
float toDegree(float radian);
float toRadian(float degree);
sf::Vector2f rotateVector(const sf::Vector2f& vec, float angle);

template <typename T>
inline std::string toString(const T& value)
{
    std::stringstream stream;
    stream << value;
    return stream.str();
}