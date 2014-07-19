#include "stdafx.h"

#include "FileSystem.h"
#include "Level.h"
#include "File.h"

#include <Box2D/Box2D.h>

using namespace bash_dogs;

bash_dogs::FileSystem::FileSystem(Level & level, const Vector& pos, bool graphics /*= true*/) :
Object(&level, pos),
graphics(graphics) {

	root = new File(*this, Vector::ZERO, "ROOT", File::T_FOLDER, File::FS_OPEN );

	if (graphics) {
		addChild(root);
		root->select(true);
	}
}

void bash_dogs::FileSystem::_makeSubFile(File& parent, int step, int& cachesPlaced, std::vector<File*>& nodes) {

	int childs = Math::rangeRandom(0, 5);

	for (int i = 0; i < childs; ++i) {

		File::Type t;
		int type = Math::rangeRandom(0, 100);
		if (type < 40 && step > 1 && cachesPlaced <= 3) {
			t = File::T_CACHE;
			cachesPlaced++;
		}
		else if (type < 70)
			t = File::T_FILE;
		else
			t = File::T_FOLDER;

		float r = 1.5f;
		Vector pos = parent.position + Math::randomVector(Vector(-r, -r, -0.2f), Vector(r, r, 0.2f));

		auto f = new File(*this, pos, "random", t, File::FS_OPEN);
		parent.addFile(*f);
		nodes.push_back(f);

		if (t == File::T_FOLDER)
			_makeSubFile(*f, step + 1, cachesPlaced, nodes);
	}
}

void bash_dogs::FileSystem::initialize() {

	std::vector<File*> nodes = { root };

	int cachesPlaced = 0;

	while (cachesPlaced < 1)
		_makeSubFile(*root, 0, cachesPlaced, nodes);

	if (!graphics)
		return;

	//relax the position of all nodes using Box2D
	b2Vec2 gravity(0, 0.f);
	b2World box2D(gravity);
	box2D.SetAllowSleeping(false);

	std::vector<b2Body*> bodies;
	for (auto f : nodes) {

		b2BodyDef def;
		def.type = b2_dynamicBody;
		def.linearDamping = 0.1f;
		def.angularDamping = 0.2f;
		def.position.Set( f->position.x, f->position.y );

		f->body = box2D.CreateBody(&def);

		b2CircleShape shape;
		shape.m_radius = 0.4f;

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;
		f->body->CreateFixture(&fixtureDef);

		bodies.push_back(f->body);
	}

	//let the thing relax the thing
	for (int i = 0; i < 200; ++i) {

		for (auto a : nodes) {
			for (auto b : nodes) {
				if ( a == b )
					continue;

				auto F = a->body->GetPosition() - b->body->GetPosition();
				float f2 = F.LengthSquared();
				F.x = 0.04f / f2;
				F.y = 0.04f / f2;

				a->body->ApplyForceToCenter(F, true);
			}
		}

		float32 timeStep = 1.0f / 60.0f;
		int32 velocityIterations = 6;
		int32 positionIterations = 2;

		box2D.Step(timeStep, velocityIterations, positionIterations);
	}

	//copy back the position
	Vector min = Vector::MAX;
	Vector max = Vector::MIN;
	for (int i = 0; i < bodies.size(); ++i) {
		nodes[i]->position.x = bodies[i]->GetPosition().x;
		nodes[i]->position.y = bodies[i]->GetPosition().y;

		min = Math::min(nodes[i]->position, min);
		max = Math::max(nodes[i]->position, max);
	}

	Vector halfDim = (max - min) * 0.5f;
	for (auto n : nodes) {
		n->position -= min + halfDim;
	}

	//center the stuff

	//make neat links
	Mesh* links = new Mesh();

	links->setVertexFieldEnabled(Mesh::VF_POSITION3D);
	links->setTriangleMode(Mesh::TM_LINE_LIST);

	links->begin();
	for (auto n : nodes) {
		for (auto c : n->getSubFiles()) {
			links->vertex(n->position);
			links->vertex(c->position);
		}
	}
	links->end();

	auto r = new Renderable(this, Vector::ZERO, links);
	addChild(r, (int)Layers::LL_GRAPH);
}

void FileSystem::onAction(float dt) {
	rotate(0.03f, Vector::NEGATIVE_UNIT_Z);

	Object::onAction(dt);
}
