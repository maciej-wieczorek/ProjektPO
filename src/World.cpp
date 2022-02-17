#include "World.hpp"
#include "config.h"
#include "Pickup.hpp"
#include "Projectile.hpp"
#include "SpriteNode.hpp"
#include "Utility.hpp"
#include "DataTables.hpp"

#include <set>

namespace
{
    const std::vector<CharacterData> Table = initializeCharacterData();
}

World::World(sf::RenderWindow& outputTarget, FontHolder& fonts) :
    mTarget{ outputTarget },
    mWorldDifficulty{ 1 },
    mWorldView{ outputTarget.getDefaultView() },
    mediaDir{ RESOURCE_PATH },
    mTextures{},
    mFonts{ fonts },
    mCollisionCountdown{ sf::Time::Zero },
    mSceneGraph{},
    mSceneLayers{},
    mPlayableArea{ mWorldView.getSize().x / 2.f, mWorldView.getSize().y / 2.f, 
                   mWorldView.getSize().x * 1.f, mWorldView.getSize().y * 1.f },
    mWorldBounds{ 0.f, 0.f, mWorldView.getSize().x * 2.f, mWorldView.getSize().y * 2.f },
    mSpawnPosition{ mWorldBounds.width / 2.f, mWorldBounds.height / 2.f },
    mPlayerCharacters{},
    mAlivePlayers{}
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
    guideEnemies();

    while (!mCommandQueue.isEmpty())
        mSceneGraph.runCommand(mCommandQueue.pop(), dt);

    adaptPlayerVelocity();
    mWorldView.setCenter(mPlayerCharacters[0]->getWorldPosition());
    handleCollisions();

    // remove killed player character pointers
    auto firstToRemove = std::remove_if(mPlayerCharacters.begin(), mPlayerCharacters.end(), std::mem_fn(&Character::isMarkedForRemoval));
    mPlayerCharacters.erase(firstToRemove, mPlayerCharacters.end());

    mSceneGraph.removeBodys();
    spawnEnemies();

    mSceneGraph.update(dt, mCommandQueue);
    adaptPlayerPosition();
    if (mCollisionCountdown > sf::Time::Zero)
        mCollisionCountdown -= dt;
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
    std::unique_ptr<Character> player(new Character(static_cast<Character::Type>(identifier), mTextures, mFonts));
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
    return mPlayerCharacters.size() > 0;
}

void World::loadTextures()
{
    mTextures.load(Textures::Entities, mediaDir + "textures/spritesheet_characters.png");
    mTextures.load(Textures::Bullet, mediaDir + "textures/pistol_bullet.png");
    mTextures.load(Textures::Floor, mediaDir + "textures/floor.png");
    mTextures.load(Textures::Grass, mediaDir + "textures/grass.png");
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
        position.x = std::max(position.x, mPlayableArea.left + borderDistance);
        position.x = std::min(position.x, mPlayableArea.left + mPlayableArea.width - borderDistance);
        position.y = std::max(position.y, mPlayableArea.top + borderDistance);
        position.y = std::min(position.y, mPlayableArea.top + mPlayableArea.height - borderDistance);
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

            if (mCollisionCountdown <= sf::Time::Zero)
            {
                player.damage(10);
                mCollisionCountdown += Table[enemy.getType()].fireInterval;
            }
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

            // Apply projectile damage
            character.damage(projectile.getDamage());
            projectile.kill();
        }
    }
}

void World::buildScene()
{
    for (int i = 0; i < static_cast<int>(Layer::LayerCount); i++)
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

    // tiled background
    sf::Texture& grassTexture = mTextures.get(Textures::Grass);
    grassTexture.setRepeated(true);
    std::unique_ptr<SpriteNode> grassSprite(new SpriteNode(grassTexture, static_cast<sf::IntRect>(mWorldBounds)));
    grassSprite->setPosition(0.f, 0.f);
    mSceneLayers[static_cast<int>(Layer::Background)]->attachChild(std::move(grassSprite));

    sf::Texture& floorTexture = mTextures.get(Textures::Floor);
    floorTexture.setRepeated(true);
    std::unique_ptr<SpriteNode> floorSprite(new SpriteNode(floorTexture, static_cast<sf::IntRect>(mPlayableArea)));
    floorSprite->setPosition(mPlayableArea.left, mPlayableArea.top);
    mSceneLayers[static_cast<int>(Layer::Background)]->attachChild(std::move(floorSprite));

    addEnemies();
}

void World::addEnemy(Character::Type type, float relX, float relY)
{
    SpawnPoint spawn(type, mSpawnPosition.x + relX, mSpawnPosition.y + relY);
    mEnemySpawnPoints.push_back(spawn);
}

void World::addEnemies()
{
    float distance = std::sqrtf(mWorldBounds.width * mWorldBounds.width +
                                mWorldBounds.height * mWorldBounds.height);
    distance /= 2.f;
    for (int i = 0; i < mWorldDifficulty * 5; i++)
    {
        float angle = randomReal(360.f);
        addEnemy(Character::Type::Enemy1, std::cosf(angle) * distance, std::sinf(angle) * distance);
    }
}

void World::spawnEnemies()
{
    while (!mEnemySpawnPoints.empty())
    {
        SpawnPoint spawn = mEnemySpawnPoints.back();

        std::unique_ptr<Character> enemy(new Character(spawn.type, mTextures, mFonts));
        enemy->setPosition(spawn.x, spawn.y);
        enemy->setTrackedCharacter(mPlayerCharacters[randomInt(mPlayerCharacters.size())]);

        mSceneLayers[static_cast<int>(Layer::Floor)]->attachChild(std::move(enemy));

        // Enemy is spawned, remove from the list to spawn
        mEnemySpawnPoints.pop_back();
    }
}

void World::guideEnemies()
{
    // Setup command that stores all alive players in mAlivePlayers
    Command alivePlayersCollector;
    alivePlayersCollector.category = static_cast<unsigned int>(Category::PlayerCharacter);
    alivePlayersCollector.action = derivedAction<Character>([this](Character& player, sf::Time)
    {
        if (!player.isKilled())
            mAlivePlayers.push_back(&player);
    });

    Command enemyGuider;
    enemyGuider.category = static_cast<unsigned int>(Category::EnemyCharacter);
    enemyGuider.action = derivedAction<Character>([this](Character& enemy, sf::Time)
    {
        if (!enemy.isKilled())
        {
            Character* prey = enemy.getTrackedCharacter();
            if (prey->isKilled())
                enemy.setTrackedCharacter(mAlivePlayers[randomInt(mAlivePlayers.size())]);
            
            sf::Vector2f enemysPosition = enemy.getWorldPosition();
            sf::Vector2f preysPosition = prey->getWorldPosition();
            sf::Vector2f direction{unitVector(sf::Vector2f(preysPosition - enemysPosition))};
            enemy.setFacingDirection(direction);
            enemy.setVelocity(direction * enemy.getMaxSpeed());
        }
    });

    mCommandQueue.push(alivePlayersCollector);
    mCommandQueue.push(enemyGuider);
    mAlivePlayers.clear();
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