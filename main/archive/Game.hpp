#pragma once
#ifndef GAME_GUARD
#define GAME_GUARD

#include <SFML\Graphics.hpp>
#include <ResourceHolder.hpp>
#include <ResourceIdentifier.hpp>
#include <World.hpp>
#include <Player.hpp>

class Game
{
public:
	Game::Game();

	void			run();

private:
	void			update(sf::Time deltaTime);
	void			processEvents();
	void			render();

	void			handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
	void			updateStatistics(sf::Time deltaTime);

private:
	sf::RenderWindow			mWindow;
	World									mWorld;
	Player								mPlayer;

	static const float		PlayerSpeed;

	sf::Time							mStatisticsUpdateTime;	
	sf::Text							mStatisticsText;
	size_t								mStatisticsTimeNumFrame;

	//ResourceHolder
	FontHolder						mStatisticsFont;

	//static
	static const sf::Time	TimePerFrame;
};

#endif GAME_GUARD