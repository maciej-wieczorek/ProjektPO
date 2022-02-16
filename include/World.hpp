#pragma once

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"
#include "Character.hpp"
#include "CommandQueue.hpp"
#include "Command.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <array>
#include <queue>

class World 
{
public:
	World(sf::RenderWindow& outputTarget, FontHolder& fonts);

	void update(sf::Time dt);
	void draw();

	sf::FloatRect getViewBounds() const;
	CommandQueue& getCommandQueue();
	Character* addCharacter(int identifier);
	void removeCharacter(int identifier);

	bool hasAlivePlayer() const;

private:
	void loadTextures();
	void adaptPlayerVelocity();
	void adaptPlayerPosition();
	void handleCollisions();

	void buildScene();
	void addEnemies();
	void spawnEnemies();
	void destroyEntitiesOutsideView();

	enum class Layer
	{
		Background,
		Floor,
		Air,
		LayerCount
	};

	struct SpawnPoint
	{
		SpawnPoint(Character::Type type, float x, float y)
			: type(type)
			, x(x)
			, y(y)
		{
		}

		Character::Type type;
		float x;
		float y;
	};

	sf::RenderWindow& mTarget;
	sf::View mWorldView;
	std::string mediaDir;
	TextureHolder mTextures;
	FontHolder& mFonts;
	SceneNode mSceneGraph;
	std::array<SceneNode*, static_cast<int>(Layer::LayerCount)> mSceneLayers;
	CommandQueue mCommandQueue;
	sf::FloatRect mWorldBounds;
	sf::Vector2f mSpawnPosition;
	std::vector<Character*> mPlayerCharacters;
	std::vector<SpawnPoint> mEnemySpawnPoints;
	std::vector<Character*> mActiveEnemies;
};

bool matchesCategories(SceneNode::Pair& colliders, Category type1, Category type2);