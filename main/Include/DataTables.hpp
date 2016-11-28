#pragma once
#ifndef DATATABLES_GUARD
#define DATATABLES_GUARD

#include <ResourceIdentifier.hpp>
#include <SFML\Graphics.hpp>
#include <vector>

#include <Aircraft.hpp>

struct Direction
{
	Direction::Direction(float angle, float distance);

	float					angle;
	float					distance;
};

struct AircraftData
{
	int											hitpoints;
	float										speed;
	Textures::ID						texture;
	sf::IntRect							textureRect;
	std::vector<Direction>	directions;
	sf::Time								fireInterval;
};

struct ProjectileData
{
	int								damage;
	float							speed;
	Textures::ID			texture;
	sf::IntRect				textureRect;
};

struct PickupData
{
	Textures::ID										texture;
	sf::IntRect											textureRect;
	std::function<void(Aircraft&)>	action;
};

struct ParticleData
{
	sf::Color				color;
	sf::Time				lifetime;
};

std::vector<AircraftData>		initializeAircraftData();
std::vector<ProjectileData> initializeProjectileData();
std::vector<PickupData>			initializePickupData();
std::vector<ParticleData>		initializeParticleData();

#endif DATATABLES_GUARD