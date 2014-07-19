#include "stdafx.h"

#include "File.h"
#include "FileSystem.h"

using namespace bash_dogs;

Mesh* bash_dogs::File::_meshForState(ResourceGroup& rg, State state, Type type) {
	switch (state)
	{
	case bash_dogs::File::FS_UNKNOWN:
		return rg.getMesh("unknownFile");
	case bash_dogs::File::FS_LOCKED:
		return rg.getMesh("lockedFile");
	case FS_DELETED:
		return rg.getMesh("deletedFile");
	case bash_dogs::File::FS_OPEN:
		switch (type)
		{
		case bash_dogs::File::T_FILE:
			return rg.getMesh("file");
		case bash_dogs::File::T_FOLDER:
			return rg.getMesh("folder");
		}
	default:
		DEBUG_FAIL("Not valid");
	}
}

bash_dogs::File::File(FileSystem& parent, const Vector& pos, const String& name, Type type, State initialState) :
Renderable(&parent, pos, _meshForState( *parent.getGameState(), initialState, type )),
state(initialState),
type(type),
name(name) {

	scale = 0;
	cullMode = RenderState::CM_DISABLED;

	label = new TextArea(this, "debugFont", pos + Vector::NEGATIVE_UNIT_Y, true);
	label->addText(name);
	label->pixelScale = 0.5f;
	parent.getGameState()->addChild(label, (int)Layers::LL_GRAPH_TEXT);
}

File::File(FileSystem& parent, const Vector& pos, Unique<Table> desc) :
File(
	parent, 
	pos,
	desc->getName(), 
	(Type)desc->getInt("type"), 
	(State)desc->getInt("state")
) {

}

Unique<Table> bash_dogs::File::serialize() const {
	auto t = make_unique<Table>();

	t->setName(name);

	t->set("state", state);
	t->set("type", type);

	return t;
}

void File::onAction(float dt) {
	label->position = getWorldPosition() + Vector(0,0.7f) * scale;

	if (scale.x < 0.7f)
		scale += dt * 1.f;

	Object::onAction(dt);
}
