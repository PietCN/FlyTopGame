#pragma once

#ifndef STATESTACK_GUARD
#define STATESTACK_GUARD

#include <SFML\Graphics.hpp>
#include <State.hpp>
#include <StateIdentifiers.hpp>
#include <ResourceIdentifier.hpp>

#include <vector>
#include <functional>
#include <map>
#include <cassert>

class StateStack : private sf::NonCopyable
{
public:
	enum Action
	{
		Push,
		Pop,
		Clear
	};

public:
	explicit StateStack::StateStack(State::Context context);

	template <typename T>
	void				registerState(States::ID stateID);

	void				update(sf::Time dt);
	void				draw();
	void				handleEvent(const sf::Event& event);

	void				pushState(States::ID stateID);
	void				popState();
	void				clearState();

	bool				isEmpty() const;

private:
	State::Ptr	createState(States::ID stateID);
	void				applyPendingChanges();

private:
	struct PendingChange
	{
		explicit PendingChange::PendingChange(Action action, States::ID stateID = States::None);

		Action			action;
		States::ID	stateID;
	};

private:
	std::vector<State::Ptr>															mStack;
	std::vector<PendingChange>													mPendingList;
	State::Context																			mContext;
	std::map<States::ID, std::function<State::Ptr()>>		mFactories;

};

/*********************************************************************************
* Verfügbarkeit eines Zustandes generieren und einer ID zuordnen
*
*********************************************************************************/
template <typename T>
void StateStack::registerState(States::ID stateID)
{
	mFactories[stateID] = [this] ()
	{
		return State::Ptr(new T(*this, mContext));
	};
}

#endif STATESTACK_GUARD