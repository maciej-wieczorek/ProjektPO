#include "Application.hpp"

#include <iostream>
#include "Utility.hpp"

Application::Application() :
	mWindow{ sf::VideoMode(1280, 720), PROJECT_NAME },
	mStatisticsText{},
	mStatisticsUpdateTime{},
	mStatisticsCountFrames{ 0 },
	mediaDir{ RESOURCE_PATH },
	mShape{ 100.0f },
	mFont{}
{
	if (!mFont.loadFromFile(mediaDir + "fonts/arial.ttf"))
	{
		// handle error
	}

	mStatisticsText.setFont(mFont);
	mStatisticsText.setFillColor(sf::Color(33, 207, 79, 255));
	mStatisticsText.setCharacterSize(11);

	mShape.setPosition(540, 260);
}

void Application::run()
{
	sf::Clock clock;

	while (mWindow.isOpen())
	{
		processInput();
		update(clock.restart());
		render();
	}
}

void Application::processInput()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			mWindow.close();
	}
}

void Application::update(sf::Time dt)
{
	updateStatistics(dt);
}

void Application::render()
{
	mWindow.clear();

	mWindow.draw(mShape);
	mWindow.draw(mStatisticsText);

	mWindow.display();
}

void Application::updateStatistics(sf::Time dt)
{
	mStatisticsUpdateTime += dt;
	mStatisticsCountFrames++;
	if (mStatisticsUpdateTime >= sf::seconds(1.0f / statisticsRefreshRate))
	{
		mStatisticsText.setString("FPS: " + toString(mStatisticsCountFrames * statisticsRefreshRate));

		mStatisticsUpdateTime -= sf::seconds(1.0f / statisticsRefreshRate);
		mStatisticsCountFrames = 0;
	}
}