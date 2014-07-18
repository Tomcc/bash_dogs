#include "stdafx.h"

#include "BashDogsGame.h"

#include "Level.h"

using namespace bash_dogs;
using namespace Dojo;

bash_dogs::BashDogsGame::BashDogsGame() :
Game("BASH_DOGS", 0,  0, Dojo::DO_LANDSCAPE_RIGHT, 1.f / 30.f) {

}

void BashDogsGame::onBegin()
{
	//load common res
	addPrefabMeshes();
	addFolder("data");
	loadResources();

	reset();
}

void BashDogsGame::reset() {
	setState(new Level(this));
}
