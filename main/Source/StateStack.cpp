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
* Zustand anhand einer ID ausw�hlen real erstellen
* 
*********************************************************************************/
State::Ptr StateStack::createState(States::ID stateID)
{
	auto found = mFactories.find(stateID);
	assert(found != mFactories.end());

	return found->second();
}

/*********************************************************************************
* geht den Zustands-Stack r�ckw�rts durch und f�hrt f�r jeden Zustand im Stack die
* handleEvent-Methode aus
*
*********************************************************************************/
void StateStack::handleEvent(const sf::Event& event)
{
	//vom h�chsten State zum untersten aufgerufen
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
* geht den Zustands-Stack r�ckw�rts durch und f�hrt f�r jeden Zustand im Stack die
* update-Methode aus
*
*********************************************************************************/
void StateStack::update(sf::Time dt)
{
	//vom h�chsten State zum untersten aufgerufen
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
* geht den Zustands-Stack vorw�rts komplett durch und f�hrt f�r jeden Zustand im
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
* �berpr�ft ob der Stack leer ist
*
*********************************************************************************/
bool StateStack::isEmpty() const
{
	return mStack.empty();
}


/*********************************************************************************
* Solange sich ein Iterator im Stack befindet d�rfen keine �nderungen am Stack
* selbst durchgef�hrt werden.
* Die Pending-Liste nimmt s�mtliche �nderungen auf und wird am Ende der Methoden
* handleEvents & update Aufgerufen
*
*********************************************************************************/
/*********************************************************************************
* �bergibt ein Push-Befehl an die Pending-Liste
*
*********************************************************************************/
void StateStack::pushState(States::ID stateID)
{
	mPendingList.push_back(PendingChange(Push, stateID));
}

/*********************************************************************************
* �bergibt ein Pop-Befehl an die Pending-Liste
*
*********************************************************************************/
void StateStack::popState()
{
	mPendingList.push_back(PendingChange(Pop));
}

/*********************************************************************************
* �bergibt ein Clear-Befehl an die Pending-Liste
*
*********************************************************************************/
void StateStack::clearState()
{
	mPendingList.push_back(PendingChange(Clear));
}

/*********************************************************************************
* ruft die anstehenden �nderungen am Stack auf und f�hrt diese aus
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