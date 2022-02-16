#include "Application.hpp"
#include "GameState.hpp"

#include <iostream>

Application::Application() :
	mWindow{ sf::VideoMode(1280, 720), PROJECT_NAME },
	mTextures{},
	mFonts{},
	mKeyBinding1{ 1 },
	mKeyBinding2{ 2 },
	mStateStack(State::Context(mWindow, mTextures, mFonts, mKeyBinding1, mKeyBinding2)),
	mStatisticsText{},
	mStatisticsUpdateTime{},
	mStatisticsCountFrames{ 0 },
	mediaDir{ RESOURCE_PATH }
{
	mWindow.setKeyRepeatEnabled(false);
	mFonts.load(Fonts::Main, mediaDir + "fonts/arial.ttf");

	mStatisticsText.setFont(mFonts.get(Fonts::Main));
	mStatisticsText.setFillColor(sf::Color(33, 207, 79, 255));
	mStatisticsText.setCharacterSize(11);

	registerStates();
	mStateStack.pushState(States::Game);
}

void Application::run()
{
	sf::Clock clock;

	while (mWindow.isOpen())
	{
		processInput();
		update(clock.restart());
		if (mStateStack.isEmpty())
			mWindow.close();
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
	mStateStack.update(dt);
	updateStatistics(dt);
}

void Application::render()
{
	mWindow.clear();

	mStateStack.draw();

	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mStatisticsText);

	mWindow.display();
}

void Application::updateStatistics(sf::Time dt)
{
	mStatisticsUpdateTime += dt;
	mStatisticsCountFrames++;
	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString("FPS: " + toString(mStatisticsCountFrames / mStatisticsUpdateTime.asSeconds()));

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsCountFrames = 0;
	}
}

void Application::registerStates()
{
	mStateStack.registerState<GameState>(States::Game);
}