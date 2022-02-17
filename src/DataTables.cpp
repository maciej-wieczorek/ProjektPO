#include "DataTables.hpp"
#include "Projectile.hpp"
#include "Character.hpp"
#include "Pickup.hpp"

std::vector<CharacterData> initializeCharacterData()
{
	std::vector<CharacterData> data(static_cast<unsigned int>(Character::Type::TypeCount));

	data[static_cast<unsigned int>(Character::Type::Character1)].hitpoints = 200;
	data[static_cast<unsigned int>(Character::Type::Character1)].speed = 200.f;
	data[static_cast<unsigned int>(Character::Type::Character1)].texture = Textures::Entities;
	data[static_cast<unsigned int>(Character::Type::Character1)].textureRect = sf::IntRect(164, 88, 49, 43);
	data[static_cast<unsigned int>(Character::Type::Character1)].fireInterval = sf::milliseconds(600);

	data[static_cast<unsigned int>(Character::Type::Enemy1)].hitpoints = 100;
	data[static_cast<unsigned int>(Character::Type::Enemy1)].speed = 50.f;
	data[static_cast<unsigned int>(Character::Type::Enemy1)].texture = Textures::Entities;
	data[static_cast<unsigned int>(Character::Type::Enemy1)].textureRect = sf::IntRect(424, 0, 35, 43);
	data[static_cast<unsigned int>(Character::Type::Enemy1)].fireInterval = sf::milliseconds(300);

	return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
	std::vector<ProjectileData> data(static_cast<unsigned int>(Projectile::Type::TypeCount));

	data[static_cast<unsigned int>(Projectile::Type::AlliedBullet)].damage = 10;
	data[static_cast<unsigned int>(Projectile::Type::AlliedBullet)].speed = 1200.f;
	data[static_cast<unsigned int>(Projectile::Type::AlliedBullet)].texture = Textures::Bullet;
	data[static_cast<unsigned int>(Projectile::Type::AlliedBullet)].textureRect = sf::IntRect(0, 0, 12, 3);

	data[static_cast<unsigned int>(Projectile::Type::EnemyBullet)].damage = 10;
	data[static_cast<unsigned int>(Projectile::Type::EnemyBullet)].speed = 300.f;
	data[static_cast<unsigned int>(Projectile::Type::EnemyBullet)].texture = Textures::Bullet;
	data[static_cast<unsigned int>(Projectile::Type::EnemyBullet)].textureRect = sf::IntRect(178, 64, 3, 14);

	return data;
}

std::vector<PickupData> initializePickupData()
{
	std::vector<PickupData> data(static_cast<unsigned int>(Pickup::Type::TypeCount));

	data[static_cast<unsigned int>(Pickup::Type::HealthRefill)].texture = Textures::Entities;
	data[static_cast<unsigned int>(Pickup::Type::HealthRefill)].textureRect = sf::IntRect(0, 64, 40, 40);
	data[static_cast<unsigned int>(Pickup::Type::HealthRefill)].action = [](Character& c) { c.heal(25); };

	data[static_cast<unsigned int>(Pickup::Type::FireRate)].texture = Textures::Entities;
	data[static_cast<unsigned int>(Pickup::Type::FireRate)].textureRect = sf::IntRect(120, 64, 40, 40);
	data[static_cast<unsigned int>(Pickup::Type::FireRate)].action = 
		std::bind(&Character::increaseFireRate, std::placeholders::_1);

	return data;
}