#pragma once

#include "config.h"
#include "Utility.hpp"

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "KeyBinding.hpp"
#include "StateStack.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

class Application
{
public:
    Application();

    void run();

private:
    void processInput();
	void update(sf::Time dt);
	void render();

    void updateStatistics(sf::Time dt);
    void registerStates();

    sf::RenderWindow mWindow;
    TextureHolder mTextures;
    FontHolder mFonts;

    sf::Text mStatisticsText;
    sf::Time mStatisticsUpdateTime;
    std::size_t mStatisticsCountFrames;
    static constexpr float statisticsRefreshRate = 60;

    std::string mediaDir;
    KeyBinding mKeyBinding1;
    KeyBinding mKeyBinding2;
    StateStack mStateStack;
};