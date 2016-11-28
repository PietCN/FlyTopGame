#include <ParticleNode.hpp>
#include <ResourceHolder.hpp>
#include <DataTables.hpp>

namespace
{
	const std::vector<ParticleData> table = initializeParticleData();
}

/*********************************************************************************
* Cstr
*
*********************************************************************************/
ParticleNode::ParticleNode(Particle::Type type, const TextureHolder& texture)
	: mParticles()
	, mTexture(texture.get(Textures::Particle))
	, mType(type)
	, mVertexArray(sf::Quads)
	, mNeedsVertexUpdate(true)
{

}

/*********************************************************************************
* Fügt einen Partikel der Particel-Queue hinzu
*
*********************************************************************************/
void ParticleNode::addParticle(sf::Vector2f position)
{
	Particle particle;

	particle.position = position;
	particle.lifetime = table[mType].lifetime;
	particle.color = table[mType].color;

	mParticles.push_back(particle);
}

/*********************************************************************************
* Gibt den Partikel - Typ zurück
*
*********************************************************************************/
Particle::Type ParticleNode::getParticleType() const
{
	return mType;
}

/*********************************************************************************
* Gibt die Kategory zurück 
*
*********************************************************************************/
size_t ParticleNode::getCategory() const {
	return Category::ParticleSystem;
}

/*********************************************************************************
* Update-Methode für den Partikel-Knoten
* Partikel die ihre Auflösungszeit erreicht haben, werden aus der Queue gelöscht
* Zeit dr wird von jedem Partikel subtrahiert
*
*********************************************************************************/
void ParticleNode::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	while (!mParticles.empty() && mParticles.front().lifetime <= sf::Time::Zero)
		mParticles.pop_front();

	for (auto& particle : mParticles)
		particle.lifetime -= dt;

	mNeedsVertexUpdate = true;
}

/*********************************************************************************
* 
*
*********************************************************************************/
void ParticleNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (mNeedsVertexUpdate)
	{
		computeVertices();
		mNeedsVertexUpdate = false;
	}

	states.texture = &mTexture;
	target.draw(mVertexArray, states);
}

/*********************************************************************************
* Fügt einen Vertex (Eckpunkt) dem VertexArray hinzu
*
*********************************************************************************/
void ParticleNode::addVertex(float worldX, float worldY, float texcoordX, float texcoordY, const sf::Color& color) const
{
	sf::Vertex vertex;
	
	vertex.position = sf::Vector2f(worldX, worldY);
	vertex.texCoords = sf::Vector2f(texcoordX, texcoordY);
	vertex.color = color;

	mVertexArray.append(vertex);
}

/*********************************************************************************
* Berechnung für alle aktiven Vertexes
*
*********************************************************************************/
void ParticleNode::computeVertices() const
{
	sf::Vector2f size(mTexture.getSize());
	sf::Vector2f half = size / 2.f;

	mVertexArray.clear();

	for (auto& particle : mParticles)
	{
		sf::Vector2f pos = particle.position;
		sf::Color c			 = particle.color;

		float ratio = particle.lifetime.asSeconds() / table[mType].lifetime.asSeconds();

		//alpha-Wert in Abhängigkeit mit der Lebenszeit des Partikels
		c.a = static_cast<sf::Uint8>(255 * std::max(ratio, 0.f));

		//VertexArray erstellen
		addVertex(pos.x - half.x, pos.y - half.y, 0.f,    0.f,    c);
		addVertex(pos.x + half.x, pos.y - half.y, size.x, 0.f,    c);
		addVertex(pos.x + half.x, pos.y + half.y, size.x, size.y, c);
		addVertex(pos.x - half.x, pos.y + half.y, 0.f,    size.y, c);
	}
}
