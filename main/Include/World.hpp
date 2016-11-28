#pragma once
#ifndef WORLD_GUARD
#define WORLD_GUARD

#include <SFML\Graphics.hpp>
#include <array>

#include <ResourceHolder.hpp>
#include <ResourceIdentifier.hpp>
#include <SceneNode.hpp>
#include <SpriteNode.hpp>
#include <Aircraft.hpp>
#include <Pickup.hpp>
#include <CommandQueue.hpp>
#include <Player.hpp>

class World : private sf::NonCopyable
{
public:
	explicit World::World(sf::RenderWindow& window, FontHolder& fonts);
	void						update(sf::Time dt);
	void						draw();
	CommandQueue&		getCommandQueue();

	bool						hasAlivePlayer() const;
	bool						hasPlayerReachedEnd() const;

private:
	void						loadTextures();
	void						buildScene();

	void						destroyEntitiesOutsideView();
	void						handleCollisions();
	void						adaptPlayerVelocity();
	void						adaptPlayerPosition();
	void						spawnEnemies();
	void						addEnemies();
	void						addEnemy(Aircraft::Type type, float x, float y);
	sf::FloatRect		getViewBounds() const;
	sf::FloatRect		getBattlefieldBounds() const;
	void						guidedMissiles();

private:
	enum Layer
	{
		Background,
		LowerAir,
		UpperAir,
		LayerCount
	};

	struct SpawnPoint
	{
		SpawnPoint::SpawnPoint(Aircraft::Type type, float x, float y);
		
		Aircraft::Type		type;
		float							x;
		float							y;
	};

private:
	sf::RenderWindow&										mWindow;
	sf::View														mWorldView;
	TextureHolder												mTextures;
	FontHolder&													mFonts;
	SceneNode														mSceneGraph;
	std::array<SceneNode*, LayerCount>	mSceneLayers;

	sf::FloatRect												mWorldBounds;
	sf::Vector2f												mSpawnPosition;
	float																mScrollSpeed;
	Aircraft*														mPlayerAircraft;
	CommandQueue												mCommandQueue;
	std::vector<SpawnPoint>							mEnemySpawnPoints;
	std::vector<Aircraft*>							mActiveEnemies;
};

bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2);

#endif WORLD_GUARD