#include <State.hpp>
#include <StateStack.hpp>

/*********************************************************************************
* Cstr - Context
* Initialisierung und Zuweisung der Globalen Member von der Application
*
*********************************************************************************/
State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, Player& player)
	: window(&window)
	, textures(&textures)
	, fonts(&fonts)
	, player(&player)
{
	
}

/*********************************************************************************
* Cstr
* Initialisierung und Zuweisung zum StateStack, Laden des Context
*
*********************************************************************************/
State::State(StateStack& stack, Context context)
	: mStack(&stack)
	, mContext(context)
{

}

/*********************************************************************************
* Leerer viruteller Dstr
*
*********************************************************************************/
State::~State()
{

}

/*********************************************************************************
* Anforderung State mittels einer ID in den StateStack zu holen
* Angeforderter State muss erst mittels ->registerState(ID)<- mit dem Typ des 
* spezifischen States und entsprechender ID auf Abruf registriert werden
*
*********************************************************************************/
void State::requestStackPush(States::ID stateID)
{
	mStack->pushState(stateID);
}

/*********************************************************************************
* Anforderung den letzten State aus dem Stack zu entfernen
* s�mtliche Daten im entfernten Stack sind gel�scht
*
*********************************************************************************/
void State::requestStackPop()
{
	mStack->popState();
}

/*********************************************************************************
* Anforderung um den gesamten Stack zu l�schen
*
*********************************************************************************/
void State::requestStackClear()
{
	mStack->clearState();
}

/*********************************************************************************
* gibt den Kontext eines States zur�ck
* enthalten sind Window, textures, fonts, player
*
*********************************************************************************/
State::Context State::getContext() const
{
	return mContext;
}