#pragma once

#include "config.h"
#include <SFML/Graphics.hpp>

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"

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

    sf::RenderWindow mWindow;

    sf::Text mStatisticsText;
    sf::Time mStatisticsUpdateTime;
    std::size_t mStatisticsCountFrames;
    static constexpr float statisticsRefreshRate = 60;

    std::string mediaDir;
    // Temp:
    sf::CircleShape mShape;

    FontHolder mFonts;
};