#include "stdafx.h"

#include "Level.h"

#include "BashDogsGame.h"
#include "Console.h"

using namespace bash_dogs;
using namespace Dojo;

Level::Level(BashDogsGame* game) :
	GameState( game )
{

}

bash_dogs::Level::~Level()
{

}

void Level::onBegin()
{
	addSubgroup((BashDogsGame*) getGame());

	Platform::getSingleton()->getRender()->getLayer((int)Layers::LL_WIREFRAME)->wireframe = true;

	loadResources();

	{
		//viewport
		float w = (float) Platform::getSingleton()->getWindowWidth();
		float r = w / (float)Platform::getSingleton()->getWindowHeight();

		Vector worldSize( 11*r, 11 );
		//Vector targetSize( 2048, 2048/r );
		Vector targetSize( 1800*r, 1800 );

		camera = new Viewport(this,
			Vector(0,0,1), 
			worldSize,
			Color::BLACK,
			70, 0.01, 1000 );

		camera->setTargetSize( targetSize );

		addChild( camera );
		setViewport( camera );
	}

	Platform::getSingleton()->getInput()->addListener(this);

	//add all existing devices 
	auto& devices = Platform::getSingleton()->getInput()->getDeviceList();

	for (auto d : devices)
		onDeviceConnected(d);

	Vector topLeft(
		-camera->getHalfSize().x + 1,
		camera->getHalfSize().y - 0.5f);

	console = new bash_dogs::Console(*this, topLeft, *devices[0]);
	addChild(console);
}

void Level::onEnd()
{
	Platform::getSingleton()->getRender()->removeAllRenderables();
	Platform::getSingleton()->getRender()->removeAllViewports();
	Platform::getSingleton()->getInput()->removeListener(this);

	for (auto d : Platform::getSingleton()->getInput()->getDeviceList() )
		d->removeListener(this);
}

//----- immediate substate events
void Level::onStateBegin()
{

}

void Level::onStateLoop( float dt )
{

}

void Level::onStateEnd()
{

}

void Level::onButtonReleased(Dojo::InputDevice* j, int action) {
	
	if (action == KC_F1) {
		((BashDogsGame*)game)->reset();
	}
}

void Level::onDeviceConnected(Dojo::InputDevice* j) {
	j->addListener(this);
}

void Level::onDeviceDisconnected(Dojo::InputDevice* j) {

}
