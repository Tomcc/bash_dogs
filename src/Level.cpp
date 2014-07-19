#include "stdafx.h"

#include "Level.h"

#include "BashDogsGame.h"
#include "Console.h"
#include "RealServer.h"
#include "Client.h"
#include "FileSystem.h"

using namespace bash_dogs;
using namespace Dojo;

Level::Level(BashDogsGame* game ) :
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
	renderer->getLayer((int)Layers::LL_GRAPH)->wireframe = true;
	
	auto layer3D = renderer->getLayer((int)Layers::LL_BACKGROUND);
	layer3D->depthCheck = false;
	layer3D->depthClear = true;
	layer3D->projectionOff = false;

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
				Color::BLACK,
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
				0xff141414,
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

	console = new bash_dogs::Console(*this, topLeft);
	addChild(console);

	auto screenMesh = getMesh("schermo_tommo");
	auto r = new Renderable(this, Vector::ZERO, screenMesh);
	r->scale = camera->getSize().y / screenMesh->getDimensions().y;
	r->setTexture(rt, 0);
	r->setTexture(getTexture("hacker_screen"), 1);
	r->setShader(getShader("consoleScreen"));

	r->getShader()->setUniformCallback("DISTORTION", [this](Renderable*) {
		return &distortion;
	});

	r->color = Color::WHITE;
	addChild(r, (int)Layers::LL_MAIN_WINDOW);


	//make lines
	{
		Mesh* gridLines = new Mesh();

		gridLines->setVertexFieldEnabled(Mesh::VF_POSITION3D);
		gridLines->setVertexFieldEnabled(Mesh::VF_COLOR);
		gridLines->setTriangleMode(Mesh::TM_LINE_LIST);

		gridLines->begin();

		float y = 0.f;
		float s = 40.f;
		float ss = s * 2;
		float a = 0.6f;
		for (int i = 0; i < (int)ss; ++i) {
			float aa = powf((1.f - ((i + 2) / ss)), 3) * a;
			gridLines->vertex(-ss, y, -i);
			gridLines->color(1, 1, 1, aa);
			gridLines->vertex(ss, y, -i);
			gridLines->color(1, 1, 1, aa);
		}

		for (int i = -(int)ss; i < (int)ss; ++i) {
			gridLines->vertex(i, y, 0.f);
			gridLines->color(1, 1, 1, 1 * a);
			gridLines->vertex(i, y, -ss);
			gridLines->color(1, 1, 1, 0);
		}

		gridLines->end();

		r = new Renderable(this, Vector(0,-7.f, 0), gridLines);
		r->scale = 3;
		addChild(r, (int)Layers::LL_BACKGROUND);


		r = new Renderable(this, Vector(0, 7.f, 0), gridLines);
		r->scale = 3;
		addChild(r, (int)Layers::LL_BACKGROUND);
	}
}

bool bash_dogs::Level::connect(const String& address) {

	if (address == String("LOCALHOST")) {
		server = make_unique<RealServer>(*this);
	}
	else {

		//try to connect
		auto client = new Client(address);
		if (client->isConnected())
			server.reset(client);
		else
			delete client;
	}

	if (server != nullptr) {
		fileSystem = new FileSystem(*this, Vector::ZERO);
		addChild(fileSystem);

		//play music
		Platform::getSingleton()->getSoundManager()->playMusic(getSound("redline"), 3.f);
	}

	return server != nullptr;
}

void Level::onEnd()
{
	Platform::getSingleton()->getRender()->removeAllRenderables();
	Platform::getSingleton()->getRender()->removeAllViewports();
	Platform::getSingleton()->getInput()->removeListener(this);
	Platform::getSingleton()->getSoundManager()->stopAllSounds();
	Platform::getSingleton()->getSoundManager()->stopMusic();

	for (auto d : Platform::getSingleton()->getInput()->getDeviceList() )
		d->removeListener(this);
}

//----- immediate substate events
void Level::onStateBegin()
{

}

void Level::onStateLoop( float dt )
{
	if (Math::oneEvery(200))
		distortion = 20;
	else if (Math::oneEvery(100))
		distortion = 10;
	else
		distortion = 2;
}

void Level::onStateEnd()
{

}

void bash_dogs::Level::onButtonPressed(Dojo::InputDevice* j, int action) {

	auto finishedLine = console->onKeyPressed(action);

	if (finishedLine) {
		DEBUG_ASSERT(server, "Do not submit commands without a Server interface");

		if (finishedLine->cmd == String("quit")) {
			getGame()->stop();
		}
		else {
			server->runCommand(finishedLine->cmd, [this](const String& replyText) {
				StringReader reader(replyText);
				Table reply;

				reply.deserialize(reader);

				//print out stuff if needed
				if (reply.existsAs("echo", Table::FT_TABLE)) {
					auto echotable = reply.getTable("echo");
					for (int i = 0; i < echotable->getAutoMembers(); ++i) {
						console->write(echotable->getString(i));
						console->newLine((i < echotable->getAutoMembers() - 1) ? String::EMPTY : console->getUserName());
					}
				}
			});
		}
	}

	SoundSet* keySound;
	if (action == KC_RETURN || action == KC_SPACE || action == KC_TAB || action == KC_RSHIFT || action == KC_LSHIFT || action == KC_BACK)
		keySound = getSound("keypress_wide");
	else
		keySound = getSound("keypress");

	Platform::getSingleton()->getSoundManager()->playSound(keySound, 0.5f);
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
