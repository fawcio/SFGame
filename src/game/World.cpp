/*
 * World.cpp
 *
 *  Created on: 16 lut 2015
 *      Author: slawek
 */

#include <cassert>
#include "World.hpp"
#include "entities/SpriteNode.hpp"
#include "entities/PlayerAircraft.hpp"

namespace sfml_playground
{

const sf::Time World::cTimePerFrame = sf::milliseconds(2);

World::World(sf::RenderWindow& window) :
		mWindow(window),
		mWorldView(window.getDefaultView()),
		mWorldBoudns(0.0f, 0.0f, mWorldView.getSize().x, 100000.0f),
		mSpawnPosition(mWorldView.getSize().x / 2.0f, mWorldBoudns.height - 100.0f),
		mScrollSpeed(-100.0f),
		mPlayerAircraft(nullptr)
{
	loadTextures();
	buildScene();

	mWorldView.setCenter(mWorldBoudns.width / 2.0f, mWorldBoudns.height - mWindow.getSize().y / 2.0f);
}

World::~World()
{
}

void World::update()
{
	mWorldView.move(0.0f, mScrollSpeed * cTimePerFrame.asSeconds());

	mScenGraph.update();
}

void World::draw()
{
	mWindow.setView(mWorldView);
	mWindow.draw(mScenGraph);

}

void World::loadTextures()
{
	mTextures.load(TextureID::eRaptor, "Resources/images/Raptor.png");
	mTextures.load(TextureID::eRaptor_shadow, "Resources/images/Raptor_shadow.png");
	mTextures.load(TextureID::eLandscape, "Resources/images/Sand-Background_blurred.png");
}

void World::buildScene()
{
	// Add layers to scene graph
	for (std::size_t i=0; i < LayerID::eLayerCount; ++i)
	{
		std::unique_ptr<SceneNode> layer(new SceneNode());
		mSceneLayers[i] = layer.get();

		mScenGraph.attachChild(std::move(layer));
	}

	sf::Texture& texture = mTextures.get(TextureID::eLandscape);
	sf::IntRect  textureRect(mWorldBoudns);
	texture.setRepeated(true);

	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(mWorldBoudns.left, mWorldBoudns.top);
	mSceneLayers[LayerID::eBackground]->attachChild(std::move(backgroundSprite));

	std::unique_ptr<PlayerAircraft> playerAircraft { new PlayerAircraft {mSpawnPosition, mScrollSpeed, mTextures} };
	mPlayerAircraft = playerAircraft.get();

	mSceneLayers[LayerID::eAir]->attachChild(std::move(playerAircraft));


}

} /* namespace sfml_playground */
