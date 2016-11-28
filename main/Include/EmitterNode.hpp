#pragma once
#ifndef EMITTERNODE_GUARD
#define EMITTERNODE_GUARD

#include <SceneNode.hpp>
#include <ParticleNode.hpp>
#include <Particle.hpp>

class EmitterNode : public SceneNode
{
public:
	explicit EmitterNode::EmitterNode(Particle::Type type);

private:
	virtual void		updateCurrent(sf::Time dt, CommandQueue& commands);
	
	void						emitParticles(sf::Time dt);

private:
	sf::Time				mAccumulatedTime;
	Particle::Type	mType;
	ParticleNode*		mParticleSystem;
};


#endif EMITTERNODE_GUARD