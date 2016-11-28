#include <World.hpp>
#include <Category.hpp>
#include <EmitterNode.hpp>
#include <Utility.hpp>


#include <iostream>

World::SpawnPoint::SpawnPoint(Aircraft::Type type, float x, float y)
	:type(type), x(x), y(y)
{
	
}

/*********************************************************************************
* World Cstr
* Initialisert die Welt und setzt den Viewport zentral auf die SpawnPosition
*********************************************************************************/
World::World(sf::RenderWindow& window, FontHolder& fonts)
	: mWindow(window)
	, mWorldView(window.getDefaultView())
	, mTextures()
	, mFonts(fonts)
	, mSceneGraph()
	, mSceneLayers()
	, mWorldBounds(0.f,0.f,mWorldView.getSize().x, 5000.0f)
	, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.F)
	, mScrollSpeed(-50.f)
	, mPlayerAircraft(nullptr)
	, mCommandQueue()
	, mEnemySpawnPoints()
{
	loadTextures();
	buildScene();

	mWorldView.setCenter(mSpawnPosition);
}

/*********************************************************************************
* WORLD UPDATE
*
*********************************************************************************/
void World::update(sf::Time dt)
{
	mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());
	mPlayerAircraft->setVelocity(0.f, 0.f);

	destroyEntitiesOutsideView();
	guidedMissiles();

	//Kommandos abrufen
	while (!mCommandQueue.isEmpty())
	{
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	}

	adaptPlayerVelocity();

	handleCollisions();
	mSceneGraph.removeWrecks();
	spawnEnemies();

	mSceneGraph.update(dt, mCommandQueue);
	
	std::set<SceneNode::Pair> collisionPairs;

	mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);
	adaptPlayerPosition();
}

/*********************************************************************************
* WORLD RENDER
*
*********************************************************************************/
void World::draw()
{
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

/*********************************************************************************
* WORLD LOAD TEXTURES
*
*********************************************************************************/
void World::loadTextures()
{
	mTextures.load(Textures::Entities,			"Media//Textures//Entities.png");
	mTextures.load(Textures::Jungle,				"Media//Textures//Jungle.png");
	mTextures.load(Textures::FinishLine,		"Media//Textures//FinishLine.png");
	mTextures.load(Textures::Explosion,			"Media//Textures//Explosion.png");
	mTextures.load(Textures::Particle,			"Media//Textures//Particle.png");
}

/*********************************************************************************
* Initialisierung der Welt
*
*********************************************************************************/
void World::buildScene()
{
	//Layers initialisieren und dem SceneGraph(Root) als Kind-Knoten hinzufügen
	for (size_t i = 0; i < LayerCount; ++i)
	{
		Category::Type category = (i == LowerAir) ? Category::SceneAirLayer : Category::None;

		SceneNode::Ptr layer(new SceneNode(category));
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}

	//Vorbereitung für den Hintergrund
	sf::Texture& texture = mTextures.get(Textures::Jungle);
	sf::IntRect textureRect(mWorldBounds);
	texture.setRepeated(true);

	//Hintergund zum Layer Background hinzufügen
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

	//Ziellinie hinzufügen
	sf::Texture& finishTexture = mTextures.get(Textures::FinishLine);
	std::unique_ptr<SpriteNode> finischSprite(new SpriteNode(finishTexture));
	finischSprite->setPosition(0.f, -76.0f);
	mSceneLayers[Background]->attachChild(std::move(finischSprite));

	//Partikel zum Layer hinzufügen (Rauch)
	std::unique_ptr<ParticleNode> smokeNode(new ParticleNode(Particle::Smoke, mTextures));
	mSceneLayers[LowerAir]->attachChild(std::move(smokeNode));

	//Partikel zum Layer hinzufügen (Treibgase)
	std::unique_ptr<ParticleNode> propellantNode(new ParticleNode(Particle::Propellant, mTextures));
	mSceneLayers[LowerAir]->attachChild(std::move(propellantNode));

	//Spieler hinzufügen mit Eskorte rechts/links
	std::unique_ptr<Aircraft> leader(new Aircraft(Aircraft::Type::Eagle, mTextures, mFonts));
	mPlayerAircraft = leader.get();
	mPlayerAircraft->setPosition(mSpawnPosition);
	mSceneLayers[UpperAir]->attachChild(std::move(leader));

	/////////////////////////////////////////////////////////////////////TEST-PICKUPS

	/*std::unique_ptr<Pickup> pickup(new Pickup(static_cast<Pickup::Type>(randomInt(Pickup::TypeCount)), mTextures));
	pickup->setPosition(mSpawnPosition - sf::Vector2f(0.f, 500.f));
	mSceneLayers[Air]->attachChild(std::move(pickup));*/

	/////////////////////////////////////////////////////////////////////TEST-PICKUPS

	//Feinde hinzufügen
	addEnemies();
}

/*********************************************************************************
* Rückgabe Kommandoschlange
*
*********************************************************************************/
CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}

/*********************************************************************************
* Berechnet die Geschwindigkeit des Spielers
*
*********************************************************************************/
void World::adaptPlayerVelocity()
{
	sf::Vector2f velocity = mPlayerAircraft->getVelocity();

	if (velocity.x != 0 && velocity.y != 0)
		mPlayerAircraft->setVelocity(velocity / std::sqrt(2.0f));

	mPlayerAircraft->accelerate(0.f, mScrollSpeed);
}

/*********************************************************************************
* Berechnung und Einhaltug der Spielerposition innerhalb des View-Bereiches
*
*********************************************************************************/
void World::adaptPlayerPosition()
{
	sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
	const float borderDistance = 40.0f;

	sf::Vector2f position = mPlayerAircraft->getPosition();

	position.x = std::max(position.x, viewBounds.left + borderDistance);										//linke Begrenzung
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance); //rechte Begrenzung
	position.y = std::max(position.y, viewBounds.top + borderDistance);											//obere Begrenzung
	position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance); //untere Begrenzung

	mPlayerAircraft->setPosition(position);
}

void World::addEnemy(Aircraft::Type type, float relX, float relY)
{

	SpawnPoint spawn(type, mSpawnPosition.x + relX, mSpawnPosition.y - relY);
	mEnemySpawnPoints.push_back(spawn);
}

/*********************************************************************************
* Liste der Hinzugefügten Gegner
*
*********************************************************************************/
void World::addEnemies()
{
	addEnemy(Aircraft::Raptor, 0.f, 500.f);
	addEnemy(Aircraft::Raptor, 0.f, 1000.f);
	addEnemy(Aircraft::Raptor, +100.f, 1150.f);
	addEnemy(Aircraft::Raptor, -100.f, 1150.f);
	addEnemy(Aircraft::Avenger, 70.f, 1500.f);
	addEnemy(Aircraft::Avenger, -70.f, 1500.f);
	addEnemy(Aircraft::Avenger, -70.f, 1710.f);
	addEnemy(Aircraft::Avenger, 70.f, 1700.f);
	addEnemy(Aircraft::Avenger, 30.f, 1850.f);
	addEnemy(Aircraft::Raptor, 300.f, 2200.f);
	addEnemy(Aircraft::Raptor, -300.f, 2200.f);
	addEnemy(Aircraft::Raptor, 0.f, 2200.f);
	addEnemy(Aircraft::Raptor, 0.f, 2500.f);
	addEnemy(Aircraft::Avenger, -300.f, 2700.f);
	addEnemy(Aircraft::Avenger, -300.f, 2700.f);
	addEnemy(Aircraft::Raptor, 0.f, 3000.f);
	addEnemy(Aircraft::Raptor, 250.f, 3250.f);
	addEnemy(Aircraft::Raptor, -250.f, 3250.f);
	addEnemy(Aircraft::Avenger, 0.f, 3500.f);
	addEnemy(Aircraft::Avenger, 0.f, 3700.f);
	addEnemy(Aircraft::Raptor, 0.f, 3800.f);
	addEnemy(Aircraft::Avenger, 0.f, 4000.f);
	addEnemy(Aircraft::Avenger, -200.f, 4200.f);
	addEnemy(Aircraft::Raptor, 200.f, 4200.f);
	addEnemy(Aircraft::Raptor, 0.f, 4400.f);


	std::sort(mEnemySpawnPoints.begin(), mEnemySpawnPoints.end(), 
		[](SpawnPoint lhs, SpawnPoint rhs) 
	{ 
		return lhs.y < rhs.y; 
	});

}

/*********************************************************************************
* Feind am Spawnpoint erscheinen lassen (Enemy Register in SceneNode)
*
*********************************************************************************/
void World::spawnEnemies()
{
	while (!mEnemySpawnPoints.empty() && mEnemySpawnPoints.back().y > getBattlefieldBounds().top)
	{
		SpawnPoint spawn = mEnemySpawnPoints.back();

		std::unique_ptr<Aircraft> enemy(new Aircraft(spawn.type, mTextures, mFonts));
		enemy->setPosition(spawn.x, spawn.y);
		enemy->setRotation(180.f);

		mSceneLayers[UpperAir]->attachChild(std::move(enemy));

		mEnemySpawnPoints.pop_back();
	}
}

/*********************************************************************************
*
*
*********************************************************************************/
sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

/*********************************************************************************
* 
*
*********************************************************************************/
sf::FloatRect World::getBattlefieldBounds() const
{
	sf::FloatRect bounds = getViewBounds();

	bounds.top -= 100.f;
	bounds.height += 100.f;

	return bounds;
}

/*********************************************************************************
*
*
*********************************************************************************/
void World::guidedMissiles()
{
	Command enemyCollector;

	enemyCollector.category = Category::EnemyAircraft;
	enemyCollector.action = derivedAction<Aircraft>([this](Aircraft& enemy, sf::Time) 
	{
		if (!enemy.isDestroyed())
			mActiveEnemies.push_back(&enemy);
	});

	Command missileGuider;
	missileGuider.category = Category::Projectile;
	missileGuider.action = derivedAction<Projectile>([this](Projectile& missile, sf::Time)
	{
		//Ignoriere ungelenkte Projektile
		if (!missile.isGuided())
			return;

		float minDistance = std::numeric_limits<float>::max();
		Aircraft* closestEnemy = nullptr;

		for (auto& enemy : mActiveEnemies)
		{
			float enemyDistance = distance(missile, *enemy);

			if (enemyDistance < minDistance)
			{
				closestEnemy = enemy;
				minDistance = enemyDistance;
			}
		}

		if (closestEnemy)			//wenn kein Gegner gefunden wurde ist closestEnemy == nullptr
			missile.guideTowards(closestEnemy->getPosition());
	});

	mCommandQueue.push(enemyCollector);
	mCommandQueue.push(missileGuider);
	mActiveEnemies.clear();
}

void World::handleCollisions()
{
	std::set<SceneNode::Pair> collisionPairs;

	mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);

	for (auto pair: collisionPairs)
	{
		//check collisions between two aircrafts
		if (matchesCategories(pair, Category::PlayerAircraft, Category::EnemyAircraft))
		{
			auto& player = static_cast<Aircraft&>(*pair.first);
			auto& enemy = static_cast<Aircraft&>(*pair.second);

			player.damage(enemy.getHitpoints());
			enemy.destroy();
		}
		//check collisions between playeraircraft and pickup
		else if (matchesCategories(pair, Category::PlayerAircraft, Category::Pickup))
		{
			auto& player = static_cast<Aircraft&>(*pair.first);
			auto& pickup = static_cast<Pickup&>(*pair.second);

			pickup.apply(player);
			pickup.destroy();
		}
		//ckeck collisions between aircraft and projectile
		else if (matchesCategories(pair, Category::EnemyAircraft, Category::AlliedProjectile) || 
			       matchesCategories(pair, Category::PlayerAircraft, Category::EnemyProjectile))
		{
			auto& aircraft   = static_cast<Aircraft&>(*pair.first);
			auto& projectile = static_cast<Projectile&>(*pair.second);

			aircraft.damage(projectile.getDamage());
			projectile.destroy();
		}
	}
}

void World::destroyEntitiesOutsideView()
{
	Command command;
	command.category = Category::Projectile | Category::EnemyAircraft;
	command.action = derivedAction<Entity>([this](Entity& e, sf::Time) 
	{
		if (!getBattlefieldBounds().intersects(e.getBoundingRect()))
		{
			e.destroy();
		}
	});

	mCommandQueue.push(command);
}

bool World::hasAlivePlayer() const
{
	return !mPlayerAircraft->isMarkedForRemoval();
}

bool World::hasPlayerReachedEnd() const
{
	return !mWorldBounds.contains(mPlayerAircraft->getPosition());
}

bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
	size_t category1 = colliders.first->getCategory();
	size_t category2 = colliders.second->getCategory();

	if (category1 & type1 && category2 & type2)
	{
		return true;
	}
	else if (category1 & type2 && category2 & type1)
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
	{
		return false;
	}
}