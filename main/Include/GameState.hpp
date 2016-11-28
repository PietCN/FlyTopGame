#pragma once
#ifndef GAMESTATE_GUARD
#define GAMESTATE_GUARD

#include <State.hpp>
#include <World.hpp>

class StateStack;
class Player;

class GameState : public State
{
public:
	GameState::GameState(StateStack& stack, Context context);

	virtual void			draw();
	virtual bool			update(sf::Time dt);
	virtual bool			handleEvent(const sf::Event& event);

private:
	World							mWorld;
	Player&						mPlayer;
};


#endif GAMESTATE_GUARD