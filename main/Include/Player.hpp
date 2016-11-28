#pragma once
#ifndef PLAYER_GUARD
#define PLAYER_GUARD

#include <SFML\Graphics.hpp>
#include <CommandQueue.hpp>


class Player
{
	//Controller für den Spieler
	//
public:
	Player::Player();

	//Mögliche Controller Eingaben
	//Letzter Wert ist gleichzeitig die Anzahl von möglichen Eingaben
	enum Action
	{
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		Fire,
		LaunchMissle,
		ActionCount     //Letztes Element
	};

	enum MissionStatus
	{
		MissionRunning,
		MissionFailure,
		MissionSuccess
	};

	void								assignKey(Action action, sf::Keyboard::Key key);
	sf::Keyboard::Key		getAssignKey(Action action) const;

	void								handleEvent(const sf::Event& event, CommandQueue& commands);
	void								handleRealTimeInput(CommandQueue& commands);
	void								setMissionStatus(MissionStatus status);
	MissionStatus				getMissionStatus() const;


private:
	static bool					isRealtimeAction(Action action);
	void								initializedAction();

private:
	std::map<sf::Keyboard::Key, Action> mKeyBinding;
	std::map<Action, Command>           mActionBinding;
	MissionStatus												mCurrentMissionStatus;
};

#endif PLAYER_GUARD