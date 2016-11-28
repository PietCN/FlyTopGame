#include <StateStack.hpp>

/*********************************************************************************
* Cstr mit Initialisierungsliste
*
*********************************************************************************/
StateStack::StateStack(State::Context context) 
	: mStack()
	, mPendingList()
	, mContext(context)
	, mFactories()
{

}

/*********************************************************************************
* Zustand anhand einer ID auswählen real erstellen
* 
*********************************************************************************/
State::Ptr StateStack::createState(States::ID stateID)
{
	auto found = mFactories.find(stateID);
	assert(found != mFactories.end());

	return found->second();
}

/*********************************************************************************
* geht den Zustands-Stack rückwärts durch und führt für jeden Zustand im Stack die
* handleEvent-Methode aus
*
*********************************************************************************/
void StateStack::handleEvent(const sf::Event& event)
{
	//vom höchsten State zum untersten aufgerufen
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		//Wenn die oberste Ebene den Spielfluss stoppt, werden keine weiteren 
		//Befehle mehr an die unteren States weitergegeben
		if (!(*itr)->handleEvent(event))
			return;
	}

	applyPendingChanges();
}

/*********************************************************************************
* geht den Zustands-Stack rückwärts durch und führt für jeden Zustand im Stack die
* update-Methode aus
*
*********************************************************************************/
void StateStack::update(sf::Time dt)
{
	//vom höchsten State zum untersten aufgerufen
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		//Wenn die oberste Ebene den Spielfluss stoppt, werden keine weiteren 
		//Befehle mehr an die unteren States weitergegeben
		if (!(*itr)->update(dt))
			break;
	}

	applyPendingChanges();
}

/*********************************************************************************
* geht den Zustands-Stack vorwärts komplett durch und führt für jeden Zustand im
* Stack die draw-Methode aus
*
*********************************************************************************/
void StateStack::draw()
{
	for (auto& itr : mStack)
	{
		itr->draw();
	}
}

/*********************************************************************************
* Überprüft ob der Stack leer ist
*
*********************************************************************************/
bool StateStack::isEmpty() const
{
	return mStack.empty();
}


/*********************************************************************************
* Solange sich ein Iterator im Stack befindet dürfen keine Änderungen am Stack
* selbst durchgeführt werden.
* Die Pending-Liste nimmt sämtliche Änderungen auf und wird am Ende der Methoden
* handleEvents & update Aufgerufen
*
*********************************************************************************/
/*********************************************************************************
* Übergibt ein Push-Befehl an die Pending-Liste
*
*********************************************************************************/
void StateStack::pushState(States::ID stateID)
{
	mPendingList.push_back(PendingChange(Push, stateID));
}

/*********************************************************************************
* Übergibt ein Pop-Befehl an die Pending-Liste
*
*********************************************************************************/
void StateStack::popState()
{
	mPendingList.push_back(PendingChange(Pop));
}

/*********************************************************************************
* Übergibt ein Clear-Befehl an die Pending-Liste
*
*********************************************************************************/
void StateStack::clearState()
{
	mPendingList.push_back(PendingChange(Clear));
}

/*********************************************************************************
* ruft die anstehenden Änderungen am Stack auf und führt diese aus
* Pennding-Liste wird am Ende geleert
*
*********************************************************************************/
void StateStack::applyPendingChanges()
{
	for (auto& change : mPendingList)
	{
		switch (change.action)
		{
		case Push:
			mStack.push_back(createState(change.stateID));
			break;
		case Pop:
			mStack.pop_back();
			break;
		case Clear:
			mStack.clear();
			break;
		}
	}

	mPendingList.clear();
}

StateStack::PendingChange::PendingChange(Action action, States::ID stateID) : action(action), stateID(stateID)
{

}