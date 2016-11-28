#pragma once
#ifndef PARTICLE_GUARD
#define PARTICLE_GUARD

#include <SFML\Graphics.hpp>

struct Particle
{
	enum Type
	{
		Propellant,
		Smoke,
		ParticelCount
	};

	sf::Vector2f	position;
	sf::Color			color;
	sf::Time			lifetime;
};


#endif 