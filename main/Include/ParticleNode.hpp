#pragma once
#ifndef PARTICLENODE_GUARD
#define PARTICLENODE_GUARD

#include <SFML\Graphics.hpp>

#include <SceneNode.hpp>
#include <Particle.hpp>
#include <ResourceIdentifier.hpp>
#include <CommandQueue.hpp>

#include <deque>

class ParticleNode : public SceneNode
{
public:
						ParticleNode::ParticleNode(Particle::Type type, const TextureHolder& texture);

	void										addParticle(sf::Vector2f position);
	Particle::Type					getParticleType() const;
	virtual size_t					getCategory() const;

private:

	virtual void						updateCurrent(sf::Time dt, CommandQueue& commands);
	virtual void						drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

	void										addVertex(float worldx, float worldy, float texcoordx, float texcoordy, const sf::Color& color) const;
	void										computeVertices() const;

private:
	std::deque<Particle>		mParticles;
	const sf::Texture&			mTexture;
	Particle::Type					mType;

	mutable sf::VertexArray	mVertexArray;
	mutable bool						mNeedsVertexUpdate;
};


#endif PARTICLENODE_GUARD