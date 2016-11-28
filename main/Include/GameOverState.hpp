#pragma once
#ifndef GAMEOVERSTATE_GUARD
#define GAMEOVERSTATE_GUARD

#include <State.hpp>
#include <Container.hpp>
#include <ResourceIdentifier.hpp>
#include <ResourceHolder.hpp>

class GameOverState : public State
{
public:
	GameOverState::GameOverState(StateStack& stack, Context context);

	virtual void						draw();
	virtual bool						update(sf::Time dt);
	virtual bool						handleEvent(const sf::Event& event);

private:
	sf::Text						mGameOverText;
	sf::Time						mElapsedTime;
};


#endif GAMEOVERSTATE_GUARD