#pragma once
#ifndef COMMAND_GUARD
#define COMMAND_GUARD

#include <functional>
#include <SFML\System\Time.hpp>

#include <Category.hpp>
#include <cassert>

class SceneNode;


struct Command
{
	Command();

	std::function<void(SceneNode&, sf::Time)>  action;
	size_t																		 category;
};

template <typename GameObject, typename Function>
std::function<void(SceneNode&, sf::Time)> derivedAction(Function fn)
{
	return [=](SceneNode& node, sf::Time dt)
	{
		//wandelt 
		assert(dynamic_cast<GameObject*>(&node) != nullptr);

		fn(static_cast<GameObject&>(node), dt);
	};
}



#endif COMMAND_GUARD