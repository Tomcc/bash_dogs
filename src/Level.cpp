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

	auto renderer = Platform::getSingleton()->getRender();
	renderer->getLayer((int)Layers::LL_WIREFRAME)->wireframe = true;

	loadResources();

	Texture* rt;
	{
		//RT
		float w = (float) Platform::getSingleton()->getWindowWidth();
		float h = (float)Platform::getSingleton()->getWindowHeight();
		float r = w / h;

		Vector worldSize( 11*r, 11 );
		//Vector targetSize( 2048, 2048/r );
		Vector targetSize(1800 * r, 1800);

		{
			camera = new Viewport(this,
				Vector(0, 0, 1),
				worldSize,
				Color::BLUE,
				70, 0.01, 1000);

			camera->setTargetSize(targetSize);
			camera->setVisibleLayers({(int)Layers::LL_MAIN_WINDOW });


			addChild(camera);
			setViewport(camera);
		}

		{
			auto RTCamera = new Viewport(this,
				Vector(0, 0, 1),
				worldSize,
				Color::BLACK,
				70, 0.01, 1000);

			RTCamera->setTargetSize(targetSize);

			rt = new Texture();
			rt->disableTiling();
			rt->disableMipmaps();
			rt->disableBilinearFiltering();
			rt->loadEmpty(w, h, GL_RGB);
			RTCamera->setRenderTarget(rt);

			Viewport::LayerList layers;
			for (int i = 0; i < (int)Layers::LL_MAIN_WINDOW; ++i)
				layers.emplace_back(i);

			RTCamera->setVisibleLayers(layers);

			addChild(RTCamera);
			renderer->addViewport(RTCamera);
		}
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

	auto screenMesh = getMesh("schermo_tommo");
	auto r = new Renderable(this, Vector(0,0,-100), screenMesh);
	r->setRotation(Vector(0,-90.f,0));
	r->scale = camera->getSize().y / screenMesh->getDimensions().y;
	r->setTexture(rt);
	r->color = Color::WHITE;
	addChild(r, (int)Layers::LL_MAIN_WINDOW);
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
