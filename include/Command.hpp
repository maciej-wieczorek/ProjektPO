#pragma once
#include "Category.hpp"

#include <SFML/System/Time.hpp>

#include <functional>

class SceneNode;

struct Command
{
	typedef std::function<void(SceneNode&, sf::Time)> Action;
	Command();
	Action action;
	unsigned int category;
};

template <class GameObject, class Function>
Command::Action derivedAction(Function fn)
{
	return [fn](SceneNode& sceneNode, sf::Time dt)
	{
		fn(static_cast<GameObject&>(sceneNode), dt);
	};
}