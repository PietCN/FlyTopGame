#include <SceneNode.hpp>
#include <Command.hpp>
#include <utility.hpp>

#include <typeinfo>
#include <iostream>

/*********************************************************************************
* Objekt SceneNode Cstr - Scene Knoten
* initialisiert den Eltern-Knoten
*********************************************************************************/
SceneNode::SceneNode(Category::Type category)
	: mParent(nullptr)
	, mChildren()
	, mDefaultCategory(category)
{
	
}


/*********************************************************************************
* F�gt dem Knoten ein Kind-Knoten hinzu
* 
*********************************************************************************/
void SceneNode::attachChild(Ptr Child)
{
	Child->mParent = this;
	mChildren.push_back(std::move(Child));
}

/*********************************************************************************
* 
*
*********************************************************************************/
SceneNode::Ptr SceneNode::detachChild(const SceneNode& Node)
{
	auto found = std::find_if(mChildren.begin(), mChildren.end(), [&](Ptr& p) -> bool {return p.get() != &Node; });
	assert(found != mChildren.end());

	Ptr result = std::move(*found);
	result->mParent = nullptr;

	mChildren.erase(found);

	return result;
}

/*********************************************************************************
* f�r ein RenderWindow bereit gestellte Draw-Methode
* ruft die Draw-Method f�r sich selbst auf und alle seiner Kinder-Knoten
*********************************************************************************/
void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	drawCurrent(target, states);
	drawChildren(target, states);
}

/*********************************************************************************
* Draw-Methode f�r eigenen Knoten
*
*********************************************************************************/
void SceneNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	//wird nur von den Render-Objekten benutzt 
}

/*********************************************************************************
* Draw-Methode f�r Kinder-Knoten
*
*********************************************************************************/
void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const Ptr& child : mChildren)
	{
		child->draw(target, states);
	}
}

/*********************************************************************************
* Update-Methode
* ruft die Update-Methode f�r sich selbst auf und alle seiner Kinder-Knoten
*********************************************************************************/
void SceneNode::update(sf::Time dt, CommandQueue& commands)
{
	updateCurrent(dt, commands);
	updateChildren(dt, commands);
}

/*********************************************************************************
* Update-Methode f�r eigenen Knoten
*
*********************************************************************************/
void SceneNode::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	//wird nur von den Render-Objekten benutzt 
}

/*********************************************************************************
* Update-Methode f�r Kinder-Knoten
*
*********************************************************************************/
void SceneNode::updateChildren(sf::Time dt, CommandQueue& commands)
{
	for (const Ptr& child : mChildren)
	{
		child->update(dt, commands);
	}
}

/*********************************************************************************
* Errechnet die Globale-Position des Knoten in Abh�ngikeit des Root-Knottens
*
*********************************************************************************/
sf::Transform SceneNode::getWorldTransform() const
{
	sf::Transform transform = sf::Transform::Identity;

	for (const SceneNode* node = this; node != nullptr; node = node->mParent)
	{
		transform = node->getTransform() * transform;
	}
	return transform;
}

/*********************************************************************************
* Wandelt die Positon in einem Vektor2f um
*
*********************************************************************************/
sf::Vector2f SceneNode::getWorldPosition() const
{
	return getWorldTransform() * sf::Vector2f();
}

/*********************************************************************************
* Wandelt die Positon in einem Vektor2f um
*
*********************************************************************************/
sf::FloatRect SceneNode::getBoundingRect() const
{
	return sf::FloatRect();
}

/*********************************************************************************
* Gibt die Kategory des Knotens zur�ck
*
*********************************************************************************/
size_t SceneNode::getCategory() const
{
	return mDefaultCategory;
}

/*********************************************************************************
* Kontrolle von eingehenden Komandos(command),
* Weiterleitung der Komandos an die Kinder-Knoten
*
*********************************************************************************/
void SceneNode::onCommand(const Command& command, sf::Time dt)
{
	if (command.category & getCategory())
		command.action(*this, dt);

	for (Ptr& child : mChildren)
	{
		child->onCommand(command, dt);
	}
}

/*********************************************************************************
* �berpr�fung Kolision zwischen *this und gegen node
* Anschlie�end �berpr�fung Kolision aller Kinder von *this und gegen node
*
*********************************************************************************/
void SceneNode::checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs)
{
	if (this != &node && collision(*this, node) && !isDestroyed() && !node.isDestroyed())
	{
		collisionPairs.insert(std::minmax(this, &node));
		/*std::cout << "Kolision zwischen " << typeid(*this).name() << "(" << this << ")" << " und " << typeid(node).name() << "(" << &node << ")" << std::endl;*/
	}

	for (auto& child : mChildren)
		child->checkNodeCollision(node, collisionPairs);
}

/*********************************************************************************
* �berpr�fung Kolision zwischen *this und gegen node
* Anschlie�end �berpr�fung Kolision aller Kinder von *this und gegen node
*
*********************************************************************************/
void SceneNode::checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs)
{
	checkNodeCollision(sceneGraph, collisionPairs);

	for (auto& child : sceneGraph.mChildren)
	{
		checkSceneCollision(*child, collisionPairs);
	}
}

bool SceneNode::isMarkedForRemoval() const
{
	//Standardm��ig zerst�re den Node wenn das Objekt zerst�rt ist
	return isDestroyed();
}

bool SceneNode::isDestroyed() const
{
	//Standardm��ig muss kein SceneNode gel�scht werden
	return false;
}

void SceneNode::removeWrecks()
{
	auto wreckfieldBegin = std::remove_if(mChildren.begin(), mChildren.end(), std::mem_fn(&SceneNode::isMarkedForRemoval));
	mChildren.erase(wreckfieldBegin, mChildren.end());

	std::for_each(mChildren.begin(), mChildren.end(),	std::mem_fn(&SceneNode::removeWrecks));
}

/*********************************************************************************
* Information �ber die Entfernungsstrecke zweier Knoten(Objekte)
*
*********************************************************************************/
float distance(const SceneNode& lhs, const SceneNode& rhs)
{
	return length(lhs.getWorldPosition() - rhs.getWorldPosition());
}

/*********************************************************************************
* �berpr�ft ob sich zwei Objekte �berschneiden
*
*********************************************************************************/
bool collision(const SceneNode& lhs, const SceneNode& rhs)
{
	return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
}

