#pragma once
#ifndef PAUSESTATE_GUARD
#define PAUSESTATE_GUARD

#include <State.hpp>

class PauseState : public State
{
public:
	PauseState::PauseState(StateStack& stack, Context context);

	virtual void				draw();
	virtual bool				update(sf::Time dt);
	virtual bool				handleEvent(const sf::Event& event);

private:
	sf::Text						mPauseText;
	sf::Text						mInstructionText;
};


#endif PAUSESTATE_GUARD