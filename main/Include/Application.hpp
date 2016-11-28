#pragma once
#ifndef APPLICATION_GUARD
#define APPLICATION_GUARD

#include <SFML\Graphics.hpp>

#include <StateStack.hpp>
#include <ResourceHolder.hpp>
#include <ResourceIdentifier.hpp>
#include <Player.hpp>

class Application
{
public:
													Application::Application();
	void										run();

private:
	void										processInput();
	void										update(sf::Time dt);
	void										render();

	void										updateStatistics(sf::Time dt);
	void										registerStates();

private:
	static const sf::Time		TimePerFrame;

	sf::RenderWindow				mWindow;
	TextureHolder						mTextures;
	FontHolder							mFonts;
	Player									mPlayer;

	StateStack							mStateStack;

	sf::Text								mStatisticsText;
	sf::Time								mStatisticsUpdateTime;
	std::size_t							mStatisticsNumFrame;

};


#endif APPLICATION_GUARD