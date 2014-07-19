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
		case T_CACHE:
			return rg.getMesh("cache");
		}
	default:
		DEBUG_FAIL("Not valid");
	}
}

bash_dogs::File::File(FileSystem& parent, const Vector& pos, const String& name, Type type, State initialState) :
Object(&parent, pos),
state(initialState),
type(type),
name(name),
fs(parent),
localPos(pos) {

	if (parent.graphics) {
		icon = new Renderable(this, Vector::ZERO, _meshForState(*parent.getGameState(), initialState, type));
		icon->scale = 0;
		icon->cullMode = RenderState::CM_DISABLED;
		parent.getGameState()->addChild(icon, (int)Layers::LL_GRAPH_WIREFRAME);

		label = new TextArea(this, "debugFont", pos + Vector::NEGATIVE_UNIT_Y, true);
		label->addText(name);
		label->pixelScale = 0.5f;
		parent.getGameState()->addChild(label, (int)Layers::LL_GRAPH);
	}
}

File::File(FileSystem& parent,Unique<Table> desc) :
File(
	parent, 
	desc->getVector("pos"),
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
	t->set("pos", position);

	return t;
}

File& bash_dogs::File::addFile(File& file) {
	DEBUG_ASSERT(type == T_FOLDER, "Files have no subfiles");

	subFiles.push_back(&file);

	if (label)
		fs.addChild(&file);

	return file;
}

void File::onAction(float dt) {
	
	icon->position = getWorldPosition();
	label->position = icon->position + Vector(0,0.35f);

	if (icon->scale.x < 0.5f)
		icon->scale += dt * 1.f;
	else {

		if (icon->scale.x > 0.501f)
			icon->scale -= dt * 1.f;

		if (icon->scale.x < 0.5f)
			icon->scale = 0.5f;

		timer += dt;
		if (timer > 60.f / 60.f) {
			icon->scale += 0.1f;
			timer = 0;
		}
	}
	Object::onAction(dt);
}
