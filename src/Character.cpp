#include "Character.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include <cmath>

namespace
{
	const std::vector<CharacterData> Table = initializeCharacterData();
}

Character::Character(Type type, const TextureHolder& textures, const FontHolder& fonts) :
	Entity(Table[static_cast<unsigned int>(type)].hitpoints),
	mType{ type },
	mSprite(textures.get(Table[static_cast<int>(type)].texture), Table[static_cast<int>(type)].textureRect),
	mFireCommand{},
	mFireCountdown{ sf::Time::Zero },
	mIsFiring{ false },
	mSpawnedPickup{ false },
	mPickupsEnabled{ true },
	mFireRateLevel{ 1 },
	mDropPickupCommand{},
	mID{ 0 }
{
	centerOrigin(mSprite);

	mFireCommand.category = static_cast<unsigned int>(Category::SceneAirLayer);
	mFireCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createBullets(node, textures);
	};

	mDropPickupCommand.category = static_cast<unsigned int>(Category::SceneAirLayer);
	mDropPickupCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createPickup(node, textures);
	};

	std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts, ""));
	mHealthDisplay = healthDisplay.get();
	attachChild(std::move(healthDisplay));

	updateTexts();
}

unsigned int Character::getCategory() const
{
	if (isAllied())
		return static_cast<unsigned int>(Category::PlayerCharacter);
	else
		return static_cast<unsigned int>(Category::EnemyCharacter);
}

sf::FloatRect Character::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Character::remove()
{
	Entity::remove();
}

bool Character::isMarkedForRemoval() const
{
	// isKilled and death animation finished
	return isKilled();
}

bool Character::isAllied() const
{
	return mType == Type::Character1 || mType == Type::Character2;
}

unsigned int Character::getID() const
{
	return mID;
}

void Character::setID(int identifier)
{
	mID = identifier;
}

Character::operator std::string() const
{
	return "Character()";
}

void Character::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	// TODO death animation
	target.draw(mSprite, states);
}

void Character::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	updateTexts();
	updateWalkAnimation();

	if (isKilled())
	{
		checkPickupDrop(commands);
		return;
	}

	checkProjectileLaunch(dt, commands);
	Entity::updateCurrent(dt, commands);
}

void Character::checkPickupDrop(CommandQueue& commands)
{
	if (!isAllied() && randomInt(3) == 0 && !mSpawnedPickup && mPickupsEnabled)
		commands.push(mDropPickupCommand);

	mSpawnedPickup = true;
}

void Character::checkProjectileLaunch(sf::Time dt, CommandQueue& commands)
{
	if (mIsFiring && mFireCountdown <= sf::Time::Zero)
	{
		commands.push(mFireCommand);
		mFireCountdown += Table[static_cast<int>(mType)].fireInterval / (mFireRateLevel + 1.f);
		mIsFiring = false;
	}
	else if (mFireCountdown > sf::Time::Zero)
	{
		mFireCountdown -= dt;
		mIsFiring = false;
	}
}

void Character::createBullets(SceneNode& node, const TextureHolder& textures) const
{
	Projectile::Type type = isAllied() ? Projectile::Type::AlliedBullet : Projectile::Type::EnemyBullet;
	// add create weapon spread?
	createProjectile(node, type, 0.0f, 0.5f, textures);
}

void Character::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const
{
	std::unique_ptr<Projectile> projectile(new Projectile(type, textures));

	// offset for weapon
	//sf::Vector2f offset(xOffset * mSprite.getGlobalBounds().width, yOffset * mSprite.getGlobalBounds().height);
	//projectile->setPosition(getWorldPosition() + offset);

	projectile->setPosition(getWorldPosition());
	projectile->setVelocity(getFacingDirection() * projectile->getMaxSpeed());
	node.attachChild(std::move(projectile));
}

void Character::createPickup(SceneNode& node, const TextureHolder& textures) const
{
}

void Character::updateTexts()
{
	// Display hitpoints
	if (isKilled())
		mHealthDisplay->setString("");
	else
		mHealthDisplay->setString(toString(getHitpoints()) + " HP");
	mHealthDisplay->setPosition(0.f, 50.f);
	mHealthDisplay->setRotation(-getRotation());
}

void Character::updateWalkAnimation()
{
	if (getVelocity() != sf::Vector2f(0.f, 0.f))
		mSprite.setRotation(unitVectorAngle(getFacingDirection()));
}

float Character::getMaxSpeed() const
{
	return Table[static_cast<unsigned int>(mType)].speed;
}

void Character::disablePickups()
{
	mPickupsEnabled = false;
}

void Character::increaseFireRate()
{
	if (mFireRateLevel < 10)
		++mFireRateLevel;
}

void Character::fire()
{
	if (Table[static_cast<int>(mType)].fireInterval != sf::Time::Zero)
		mIsFiring = true;
}
