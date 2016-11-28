#pragma once
#ifndef SCENENODE_GUARD
#define SCENENODE_GUARD

#include "..//Debug.hpp"
#include <SFML\Graphics.hpp>
#include <memory>
#include <vector>
#include <cassert>
#include <set>

#include <Category.hpp>

struct Command;
class CommandQueue;

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
	using Ptr = std::unique_ptr<SceneNode>;
	using Pair = std::pair<SceneNode*, SceneNode*>;

public:
	explicit							SceneNode::SceneNode(Category::Type category = Category::None);

	void									attachChild(Ptr Child);
	Ptr										detachChild(const SceneNode& Node);

	void									update(sf::Time dt, CommandQueue& commands);

	sf::Transform					getWorldTransform() const;
	sf::Vector2f					getWorldPosition() const;

	void									onCommand(const Command& command, sf::Time dt);
	virtual size_t				getCategory() const;

	virtual sf::FloatRect getBoundingRect() const;
	virtual bool					isDestroyed() const;
	virtual bool					isMarkedForRemoval() const;
	void									checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs);
	void									checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs);
	void									removeWrecks();


private:
	virtual void					updateCurrent(sf::Time dt, CommandQueue& commands);
					void					updateChildren(sf::Time dt, CommandQueue& commands);
	
	virtual void					draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void					drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
					void					drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	std::vector<Ptr>			mChildren;
	SceneNode*					mParent;
	Category::Type				mDefaultCategory;
};

float distance(const SceneNode& lhs, const SceneNode& rhs);
bool collision(const SceneNode& lhs, const SceneNode& rhs);

#endif SCENENODE_GUARD