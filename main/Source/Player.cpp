#include <Player.hpp>
#include <CommandQueue.hpp>
#include <Aircraft.hpp>

#include <algorithm>

/*********************************************************************************
* Funktionsobjekt für die Spielerbewegung
*
*********************************************************************************/
struct AircraftMoveer
{
	AircraftMoveer(float vx, float vy)
		: velocity(vx, vy)
	{
		
	}

	void operator() (Aircraft& aircraft, sf::Time time) const
	{
		aircraft.accelerate(velocity);
	}

	sf::Vector2f velocity;
};

/*********************************************************************************
* Cstr
*
*********************************************************************************/
Player::Player()
	: mCurrentMissionStatus(MissionRunning)
{
	//Festsetzen der Key-Bindings
	mKeyBinding[sf::Keyboard::Up] = MoveUp;
	mKeyBinding[sf::Keyboard::Down] = MoveDown;
	mKeyBinding[sf::Keyboard::Right] = MoveRight;
	mKeyBinding[sf::Keyboard::Left] = MoveLeft;
	mKeyBinding[sf::Keyboard::Space] = Fire;
	mKeyBinding[sf::Keyboard::M] = LaunchMissle;

	initializedAction();
	//Kategorien für die Actions-Funktionen festlegen (alle auf den Spieler)
	for (auto &pair : mActionBinding)
		pair.second.category = Category::PlayerAircraft;
}

/*********************************************************************************
* Fügt Events der MessageSchalge hinzu
*
*********************************************************************************/
void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		auto found = mKeyBinding.find(event.key.code);
		if (found != mKeyBinding.end() && !isRealtimeAction(found->second))
			commands.push(mActionBinding[found->second]);
	}
}

/*********************************************************************************
* Fügt RealTimeEvents der MessageSchlange hinzu
*
*********************************************************************************/
void Player::handleRealTimeInput(CommandQueue& commands)
{
	for (auto &pair : mKeyBinding)
	{
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
			commands.push(mActionBinding[pair.second]);
	}

}

/*********************************************************************************
* Überprüfung ob die Action eine RealTime Eingabe ist
*
*********************************************************************************/
bool Player::isRealtimeAction(Action action)
{
	switch (action)
	{
	case MoveLeft:
	case MoveRight:
	case MoveUp:
	case MoveDown:
	case Fire:
		return true;

	default:
		return false;
	}
}

/*********************************************************************************
* 
*
*********************************************************************************/
void Player::setMissionStatus(MissionStatus status)
{
	mCurrentMissionStatus = status;
}

/*********************************************************************************
*
*
*********************************************************************************/
Player::MissionStatus Player::getMissionStatus() const
{
	return mCurrentMissionStatus;
}

/*********************************************************************************
* neue Taste einer action hinzufügen
* falls die Taste schon belegt ist, wird diese Zuweisung gelöscht 
*
*********************************************************************************/
void Player::assignKey(Action action, sf::Keyboard::Key key)
{
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); )
	{
		if (itr->second == action)
			mKeyBinding.erase(itr++);
		else
			itr++;
	}

	mKeyBinding[key] = action;
}

/*********************************************************************************
* gibt die Taste zurück, welche zur gesuchten Action gebunden ist
* 
*********************************************************************************/
sf::Keyboard::Key Player::getAssignKey(Action action) const
{
	for (auto &pair : mKeyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
}

/*********************************************************************************
* Initialisiert die ActionBindings
* entsprechende Funktion wird der Taste zugewiesen
*
*********************************************************************************/
void Player::initializedAction()
{
	const float playerSpeed = 200.0f;

	mActionBinding[MoveUp].action					 = derivedAction<Aircraft>(AircraftMoveer(0.f, -playerSpeed));
	mActionBinding[MoveDown].action				 = derivedAction<Aircraft>(AircraftMoveer(0.f, playerSpeed));
	mActionBinding[MoveRight].action			 = derivedAction<Aircraft>(AircraftMoveer(playerSpeed, 0.f));
	mActionBinding[MoveLeft].action				 = derivedAction<Aircraft>(AircraftMoveer(-playerSpeed, 0.f));
	mActionBinding[Fire].action						 = derivedAction<Aircraft>(std::bind(&Aircraft::fire, std::placeholders::_1));
	mActionBinding[LaunchMissle].action		 = derivedAction<Aircraft>(std::bind(&Aircraft::launchMissle, std::placeholders::_1));
}