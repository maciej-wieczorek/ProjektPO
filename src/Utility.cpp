#include "Utility.hpp"

#include <random>
#include <cassert>

namespace
{
	std::default_random_engine createRandomEngine()
	{
		auto seed = static_cast<unsigned long>(std::time(nullptr));
		return std::default_random_engine(seed);
	}

	auto RandomEngine = createRandomEngine();
}

void centerOrigin(sf::Sprite& sprite)
{
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

void centerOrigin(sf::Text& text)
{
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

int randomInt(int exclusiveMax)
{
	std::uniform_int_distribution<> distr(0, exclusiveMax - 1);
	return distr(RandomEngine);
}

float randomReal(float exclusiveMax)
{
	std::uniform_real_distribution<> distr(0, exclusiveMax);
	return static_cast<float>(distr(RandomEngine));
}

int randomIntSigned(int exclusiveMax)
{
	std::uniform_int_distribution<> distr(-exclusiveMax, exclusiveMax);
	return distr(RandomEngine);
}

float length(sf::Vector2f vector)
{
	return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

sf::Vector2f unitVector(sf::Vector2f vector)
{
	if (vector == sf::Vector2f(0.f, 0.f))
		return vector;
	return vector / length(vector);
}

float unitVectorAngle(const sf::Vector2f& vector)
{
	return std::atan2f(vector.y, vector.x) * 180.f / PI;
}

float vectorAngle(const sf::Vector2f& vector)
{
	return unitVectorAngle(unitVector(vector));
}

float toDegree(float radian)
{
	return 180.f / PI * radian;
}

float toRadian(float degree)
{
	return PI / 180.f * degree;
}

sf::Vector2f rotateVector(const sf::Vector2f& vec, float angle)
{
	sf::Vector2f vector;
	angle = toRadian(angle);
	vector.x = std::cosf(angle) * vec.x - std::sinf(angle) * vec.y;
	vector.y = std::sinf(angle) * vec.x + std::cosf(angle) * vec.y;

	return vector;
}