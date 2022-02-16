#pragma once

#include "ResourceIdentifiers.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Time.hpp>

#include <vector>
#include <functional>

class Character;

struct CharacterData
{
	int hitpoints;
	float speed;
	Textures texture;
	sf::IntRect	textureRect;
	sf::Time fireInterval;
};

struct ProjectileData
{
	int damage;
	float speed;
	Textures texture;
	sf::IntRect textureRect;
};

struct PickupData
{
	std::function<void(Character&)>	action;
	Textures texture;
	sf::IntRect textureRect;
};

std::vector<ProjectileData> initializeProjectileData();
std::vector<CharacterData> initializeCharacterData();
std::vector<PickupData> initializePickupData();