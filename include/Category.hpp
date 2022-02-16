#pragma once

enum class Category
{
	None				= 0,
	SceneAirLayer		= 1 << 0,
	PlayerCharacter		= 1 << 1,
	EnemyCharacter		= 1 << 2,
	Pickup				= 1 << 3,
	AlliedProjectile	= 1 << 4,
	EnemyProjectile		= 1 << 5,

	Character = PlayerCharacter | EnemyCharacter,
	Projectile = AlliedProjectile | EnemyProjectile,
};