#include "World.hpp"
#include "config.h"
#include "Pickup.hpp"
#include "Projectile.hpp"

#include <set>

World::World(sf::RenderWindow& outputTarget, FontHolder& fonts) :
    mTarget{ outputTarget },
    mWorldView{ outputTarget.getDefaultView() },
    mediaDir{ RESOURCE_PATH },
    mTextures{},
    mFonts{ fonts },
    mSceneGraph{},
    mSceneLayers{},
    mWorldBounds{ 0.f, 0.f, mWorldView.getSize().x * 2.f, mWorldView.getSize().y * 2.f },
    mSpawnPosition{ mWorldBounds.width / 2.f, mWorldBounds.height / 2.f },
    mPlayerCharacters{},
    mActiveEnemies{}
{
    loadTextures();
    buildScene();

    mWorldView.setCenter(mSpawnPosition);
}

void World::update(sf::Time dt)
{
    for (Character* c : mPlayerCharacters)
        c->setVelocity(0.f, 0.f);

    destroyEntitiesOutsideView();

    while (!mCommandQueue.isEmpty())
        mSceneGraph.runCommand(mCommandQueue.pop(), dt);

    adaptPlayerVelocity();
    handleCollisions();

    mSceneGraph.removeWrecks();

    mSceneGraph.update(dt, mCommandQueue);
    adaptPlayerPosition();
}

void World::draw()
{
    mTarget.setView(mWorldView);
    mTarget.draw(mSceneGraph);
}

sf::FloatRect World::getViewBounds() const
{
    return sf::FloatRect();
}

CommandQueue& World::getCommandQueue()
{
    return mCommandQueue;
}

Character* World::addCharacter(int identifier)
{
    std::unique_ptr<Character> player(new Character(Character::Type::Character1, mTextures, mFonts));
    player->setPosition(mWorldView.getCenter());
    player->setID(identifier);

    mPlayerCharacters.push_back(player.get());
    mSceneLayers[static_cast<int>(Layer::Floor)]->attachChild(std::move(player));
    return mPlayerCharacters.back();
}

void World::removeCharacter(int identifier)
{
}

bool World::hasAlivePlayer() const
{
    return false;
}

void World::loadTextures()
{
    mTextures.load(Textures::Entities, mediaDir + "textures/spritesheet_characters.png");
    mTextures.load(Textures::Bullet, mediaDir + "textures/pistol_bullet.png");
}

void World::adaptPlayerVelocity()
{
    for(Character * character : mPlayerCharacters)
    {
        sf::Vector2f velocity = character->getVelocity();

        // If moving diagonally, reduce velocity (to always have the same velocity)
        if (velocity.x != 0.f && velocity.y != 0.f)
            character->setVelocity(velocity / std::sqrt(2.f));
    }
}

void World::adaptPlayerPosition()
{
    // Keep player's position inside the world border
    const float borderDistance = 2.f;

    for (Character * character : mPlayerCharacters)
    {
        sf::Vector2f position = character->getPosition();
        position.x = std::max(position.x, mWorldBounds.left + borderDistance);
        position.x = std::min(position.x, mWorldBounds.left + mWorldBounds.width - borderDistance);
        position.y = std::max(position.y, mWorldBounds.top + borderDistance);
        position.y = std::min(position.y, mWorldBounds.top + mWorldBounds.height - borderDistance);
        character->setPosition(position);
    }
}

void World::handleCollisions()
{
    std::set<SceneNode::Pair> collisionPairs;
    mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);

    for (SceneNode::Pair pair : collisionPairs)
    {
        if (matchesCategories(pair, Category::PlayerCharacter, Category::EnemyCharacter))
        {
            auto& player = static_cast<Character&>(*pair.first);
            auto& enemy = static_cast<Character&>(*pair.second);

            player.damage(10);
        }

        else if (matchesCategories(pair, Category::PlayerCharacter, Category::Pickup))
        {
            auto& player = static_cast<Character&>(*pair.first);
            auto& pickup = static_cast<Pickup&>(*pair.second);

            // Apply pickup effect to player, destroy projectile
            pickup.apply(player);
            pickup.kill();
        }

        else if (matchesCategories(pair, Category::EnemyCharacter, Category::AlliedProjectile) ||
                 matchesCategories(pair, Category::PlayerCharacter, Category::EnemyProjectile))
        {
            auto& character = static_cast<Character&>(*pair.first);
            auto& projectile = static_cast<Projectile&>(*pair.second);

            // Apply projectile damage to aircraft, destroy projectile
            character.damage(projectile.getDamage());
            projectile.kill();
        }
    }
}

void World::buildScene()
{
    for (std::size_t i = 0; i < static_cast<int>(Layer::LayerCount); i++)
    {
        Category category;
        if (i == static_cast<int>(Layer::Air))
            category = Category::SceneAirLayer;
        else
            category = Category::None;
        SceneNode::Ptr layer(new SceneNode(category));
        mSceneLayers[i] = layer.get();

        mSceneGraph.attachChild(std::move(layer));
    }
    addEnemies();
}

void World::addEnemies()
{
}

void World::spawnEnemies()
{
}

void World::destroyEntitiesOutsideView()
{
    Command command;
    command.category = static_cast<unsigned int>(Category::Projectile);
    command.action = derivedAction<Entity>([this](Entity& e, sf::Time)
        {
            if (!mWorldBounds.intersects(e.getBoundingRect()))
                e.remove();
        });

    mCommandQueue.push(command);
}

bool matchesCategories(SceneNode::Pair& colliders, Category type1, Category type2)
{
    unsigned int category1 = colliders.first->getCategory();
    unsigned int category2 = colliders.second->getCategory();

    // Make sure first pair entry has category type1 and second has type2
    if (static_cast<unsigned int>(type1) & category1 && static_cast<unsigned int>(type2) & category2)
    {
        return true;
    }
    else if (static_cast<unsigned int>(type1) & category2 && static_cast<unsigned int>(type2) & category1)
    {
        std::swap(colliders.first, colliders.second);
        return true;
    }
    else
    {
        return false;
    }
}