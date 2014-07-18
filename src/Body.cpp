#include "stdafx.h"

#include "Body.h"
#include "Level.h"
#include "Material.h"

using namespace Scavenger;
using namespace Dojo;
using namespace Phys;

Body::Body() {

}

void Scavenger::Phys::Body::_addShape(b2Shape& shape, const Material& material, bool sensor) {
	DEBUG_ASSERT(material.solid(), "Can't add a physics shape for a non-solid material");

	b2FixtureDef fixtureDef;

	fixtureDef.shape = &shape;
	fixtureDef.density = material.density;
	fixtureDef.friction = material.friction;
	fixtureDef.restitution = material.restitution;
	fixtureDef.filter = b2Filter();
	fixtureDef.filter.groupIndex = group;
	fixtureDef.isSensor = sensor;

	fixtureDef.userData = this;

	body->CreateFixture(&fixtureDef);
}

void Body::addPolyShape(const Material& material, const std::vector<b2Vec2>& points /*= nullptr*/, bool sensor /*= false */) {
	
	b2PolygonShape shape;
	shape.Set(points.data(), points.size());

	_addShape(shape, material, sensor);
}

void Scavenger::Phys::Body::addBoxShape(const Material& material, const Vector& dimensions, const Vector& center /*= Vector::ZERO*/, bool sensor /*= false*/) {

	DEBUG_ASSERT(dimensions.x >= 0 && dimensions.y >= 0, "Invalid dimensions");

	std::vector<b2Vec2> points;

	Vector min = center - dimensions * 0.5f;
	Vector max = center + dimensions * 0.5f;

	points.emplace_back(min.x, min.y);
	points.emplace_back(min.x, max.y);
	points.emplace_back(max.x, max.y);
	points.emplace_back(max.x, min.y);

	addPolyShape(material, points, sensor);
}

void Scavenger::Phys::Body::addCircleShape(const Material& material, float radius, const Vector& center, bool sensor /*= false*/) {
	DEBUG_ASSERT(radius > 0, "Invalid radius");

	b2CircleShape circle;
	circle.m_radius = radius;
	circle.m_p.x = center.x;
	circle.m_p.y = center.y;

	_addShape(circle, material, sensor);
}

void Body::initPhysics(Dojo::Renderable* g, Group physGroup) {

	graphics = g;
	group = physGroup;

	auto& box2D = ((Level*) graphics->getGameState())->getBox2D();

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;

	bodyDef.position.Set( g->position.x, g->position.y );
	bodyDef.angularDamping = 0.1;
	bodyDef.linearDamping = 0.1;
	bodyDef.bullet = true;

	body = box2D.CreateBody(&bodyDef);

	body->SetUserData(this);
}

void Body::initPhysics(Dojo::Renderable* graphics, Body* partOf /* = nullptr */, bool fixed /* = false */) {

	if (!fixed) {
		initPhysics(graphics, partOf->getGroup() );
	}
	else {
		DEBUG_TODO;
	}
}

void Body::updatePhysics() {

	b2Transform t = body->GetTransform();

	graphics->position.x = t.p.x;
	graphics->position.y = t.p.y;

	auto speed = body->GetLinearVelocity();
	graphics->speed = Vector(speed.x, speed.y);

	graphics->setRotation(Vector(0, 0, t.q.GetAngle()));
}

void Scavenger::Phys::Body::addForce(const Vector& force)
{
	b2Vec2 f(force.x, force.y);
	body->ApplyForceToCenter(f, true);
}

float Body::getMass() const {
	return body->GetMass();
}

Dojo::Vector Scavenger::Phys::Body::getPosition() const {
	auto b2p = body->GetWorldCenter();
	return Vector(b2p.x, b2p.y);
}

void Body::onCollision(Body* other, float force) {

}