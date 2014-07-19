#include "stdafx.h"

#include "FileSystem.h"
#include "Level.h"
#include "File.h"

using namespace bash_dogs;

bash_dogs::FileSystem::FileSystem(Level & level, const Vector& pos) :
Object(&level, pos) {

	root = new File(*this, Vector::ZERO, "ROOT", File::T_FILE, File::FS_OPEN );
	addChild(root, (int)Layers::LL_GRAPH);
}

void FileSystem::onAction(float dt) {
	root->rotate(0.2f, Vector(1, 7, 0).normalized());

	Object::onAction(dt);
}
