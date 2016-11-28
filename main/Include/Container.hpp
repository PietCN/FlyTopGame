#pragma once
#ifndef CONTAINER_GUARD
#define CONTAINER_GUARD

#include <Component.hpp>
#include <vector>

namespace GUI
{
	class Container : public Component
	{
	public:
		using Ptr = std::shared_ptr<Container>;

	public:
											 Container::Container();

						void									pack(Component::Ptr component);
		virtual bool									isSelectable() const;
		virtual void									handleEvent(const sf::Event& event);

	private:
		virtual void									draw(sf::RenderTarget& target, sf::RenderStates states) const;

						bool									hasSelection() const;
						void									select(std::size_t index);
						void									selectNext();
						void									selectPrevious();

	private:
		std::vector<Component::Ptr>		mChildren;
		int														mSelectChild;

	};
}



#endif CONTAINER_GUARD