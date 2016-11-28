#include <Container.hpp>
#include <iostream>

namespace GUI
{
	/*********************************************************************************
	* Cstr
	* 
	*
	*********************************************************************************/
	Container::Container()
		: mChildren()
		, mSelectChild(-1)
	{

	}

	/*********************************************************************************
	* Ein neuer Bestandteil wird in die Komponentenliste eingetragen
	* Wenn noch keine Komponente im Container existiert und der die neue Komponente
	* anw�hlbar ist, wird diese ausgew�hlt (selectiert)
	*
	*********************************************************************************/
	void Container::pack(Component::Ptr component)
	{
		mChildren.push_back(component);
		if (!hasSelection() && component->isSelectable())
			select(mChildren.size() - 1);
	}

	/*********************************************************************************
	* Ein Container ist nicht ausw�hlbar.
	*
	*********************************************************************************/
	bool Container::isSelectable() const
	{
		return false;
	}

	/*********************************************************************************
	* Steuerung der Men�struktur �ber die Tastatur
	*
	*********************************************************************************/
	void Container::handleEvent(const sf::Event& event)
	{
		//Wenn Elemente im Container existieren und das angew�hlte Element aktiv ist
		//wird der EventHandler vom gew�hltem Komponent ausgef�hrt
		if (hasSelection() && mChildren[mSelectChild]->isActive())
			mChildren[mSelectChild]->handleEvent(event);

		else if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
			{
				selectPrevious();
			}
			else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
			{
				selectNext();
			}
			else if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)
			{
				if (hasSelection())
				{
					mChildren[mSelectChild]->activate();
				}
			}
		}
	}

	/*********************************************************************************
	* gibt an ob anw�hlbare Objekte im Container existieren
	*
	*********************************************************************************/
	bool Container::hasSelection() const
	{
		return mSelectChild >= 0;
	}

	/*********************************************************************************
	* Wenn Auswahl w�hlbar ist, wird die aktive Auswahl deaktiviert und die neue 
	* Auswahl aktiev gestellt
	*
	*********************************************************************************/
	void Container::select(std::size_t index)
	{
		if (mChildren[index]->isSelectable())
		{
			if (hasSelection())
				mChildren[mSelectChild]->deselect();
			mChildren[index]->select();
			mSelectChild = index;
		}
	}

	/*********************************************************************************
	* 
	*
	*********************************************************************************/
	void Container::selectNext()
	{
		if (!hasSelection())
			return;

		int next = mSelectChild;
		do
			next = (next + 1) % mChildren.size();
		while (!mChildren[next]->isSelectable());

		select(next);
	}

	/*********************************************************************************
	*
	*
	*********************************************************************************/
	void Container::selectPrevious()
	{
		if (!hasSelection())
			return;

		int prev = mSelectChild;
		do
			prev = (prev + mChildren.size() - 1) % mChildren.size();
		while (!mChildren[prev]->isSelectable());

		select(prev);
	}

	/*********************************************************************************
	* �bergibt die altuellen Zust�nde und f�hrt die Draw-Methoden der "Kinder" aus
	*
	*********************************************************************************/
	void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		for (auto& child : mChildren)
		{
			target.draw(*child, states);
		}
	}
}

