#include "stdafx.h"

#include "File.h"
#include "FileSystem.h"

using namespace bash_dogs;

Mesh* bash_dogs::File::_meshForState(ResourceGroup& rg, State state) {
	switch (state)
	{
	case bash_dogs::File::LL_UNKNOWN:
		return rg.getMesh("unknownFile");
	case bash_dogs::File::LL_LOCKED:
		return rg.getMesh("lockedFile");
	case bash_dogs::File::LL_FOLDER:
		return rg.getMesh("folder");
	case bash_dogs::File::LL_FILE:
		return rg.getMesh("file");
	default:
		DEBUG_FAIL("Not valid");
	}
}

bash_dogs::File::File(FileSystem& parent, const Vector& pos, const String& name, State initialState) :
Renderable(&parent, pos, _meshForState( *parent.getGameState(), initialState )),
state(initialState) {

	scale = 0;
	cullMode = RenderState::CM_DISABLED;

	label = new TextArea(this, "debugFont", pos + Vector::NEGATIVE_UNIT_Y, true);
	label->addText(name);
	label->pixelScale = 0.5f;
	parent.getGameState()->addChild(label, (int)Layers::LL_GRAPH_TEXT);
}

void File::onAction(float dt) {
	label->position = getWorldPosition() + Vector(0,0.7f) * scale;

	if (scale.x < 0.7f)
		scale += dt * 1.f;

	Object::onAction(dt);
}
