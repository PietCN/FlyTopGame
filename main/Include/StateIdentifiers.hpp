#pragma once
#ifndef STATEIDENTIFIER_GUARD
#define STATEIDENTIFIER_GUARD

/*********************************************************************************
* Beinhaltet alle m�glichen Zustand-ID's
* wird verwendet im StateStack
* 
*********************************************************************************/
namespace States
{
	enum ID
	{
		None,
		Title,
		Menu,
		Game,
		Pause,
		Setting,
		GameOver
	};
}

#endif STATEIDENTIFIER_GUARD