#include "stdafx.h"

#include "Player.h"
#include "Level.h"
#include "Material.h"

using namespace Scavenger;
using namespace Phys;

Scavenger::Player::Player(GameState& gameState, const Vector& pos) :
Renderable(&gameState, pos, "texturedQuad")
{
	setSize(0.89f, 1.5f);
	scale = getSize();

	Material player;
	player.density = 1.f;
	player.friction = 0.8f;
	player.restitution = 0.0f;

	initPhysics(this, Phys::Group::Actor);

	float r = getHalfSize().x;
	Vector offset(0, getHalfSize().y - r);

	addCircleShape(player, r, offset);
	addCircleShape(player, r, -offset);

	torch = new DynamicLight(gameState, Vector::ZERO, this);
	torch->addLayer(Color(0.15f, 0.15f, 0.25f, 1.f), 8.f);
	//torch->addLayer(Color(0.12f, 0.12f, 0.1f, 1.f), 7.f, Vector::NEGATIVE_UNIT_X, 0.1f);
	gameState.addChild(torch);
	
	setState(PS_IDLE);
}

void Player::onDestruction() {
	if (device)
		device->removeListener(this);
}

void Scavenger::Player::assignDevice(InputDevice* newDevice) {
	if (device != newDevice) {
		if (device)
			device->removeListener(this);

		device = newDevice;

		if (device)
			device->addListener(this);
	}
}


bool Scavenger::Player::_checkGround() {
	auto result = ((Level*)getGameState())->raycast(position, position - Vector::UNIT_Y * getHalfSize().y * 1.1f, this);

	return result.hit;
}

bool Player::_move(float maxSpeed, float push) {

	float force = device->getAxis(InputDevice::AI_LX);

	if (force) {
		push *= 1.f - std::min(1.f, (std::abs(speed.x) / maxSpeed));

		addForce(Vector(force*push, 0));
		return true;
	}
	else return false;
}

void Scavenger::Player::onStateBegin() {
	if (isCurrentState(PS_JUMPING)) {
		float force = doubleJump ? 80 : 40;
		addForce(Vector::UNIT_Y * force);
		jumpBoostCountDown = 5;
	}
}


void Scavenger::Player::onStateLoop(float dt) {
	bool onGround = _checkGround();

	if (isCurrentState(PS_WALKING)) {

		if (!onGround)
			setState(PS_FALLING);
		else if (!_move(4.f, 40.f))
			setState(PS_IDLE);
	}
	else if (isCurrentState(PS_IDLE)) {

		//get to stop
		float brake = -speed.x * 0.3f;
		addForce(Vector(brake, 0));

		if (!onGround)
			setState(PS_FALLING);
	}
	else if (isCurrentState(PS_JUMPING)) {
		_move(4.f, 40.f);

		//post-jump boost
		if (jumpBoostCountDown-- && device->isKeyDown(KEY_JUMP))
			addForce(Vector::UNIT_Y * 30);

		else
			jumpBoostCountDown = 0; //can't anymore

		if (speed.y <= 0)
			setState(PS_FALLING);
	}
	else if (isCurrentState(PS_FALLING)) {
		_move(4.f, 40.f);

		if (onGround) {
			if (doubleJump) { //bounce
				setState(PS_JUMPING);
			}
			else
				setState(device->getAxis(InputDevice::AI_LX) ? PS_WALKING : PS_IDLE);
		}

		doubleJump = false;
	}
}

void Scavenger::Player::onStateEnd() {

}

void Scavenger::Player::onAction(float dt) {
	//HACK should this have a superclass?
	updatePhysics();
	Vector s = speed;

	Renderable::onAction(dt);

	speed = s;

	loop(dt);
}


void Scavenger::Player::onCollision(Body* other, float force) {

}

void Scavenger::Player::onButtonPressed(Dojo::InputDevice* j, int action) {
	if (isCurrentState(PS_WALKING) || isCurrentState(PS_IDLE) ) {
		if (action == KEY_JUMP) {
			setState(PS_JUMPING);
		}
	}
	else if (isCurrentState(PS_FALLING)) {
		doubleJump = true;
	}
}

void Scavenger::Player::onButtonReleased(Dojo::InputDevice* j, int action) {

}

void Scavenger::Player::onAxisMoved(Dojo::InputDevice* j, Dojo::InputDevice::Axis a, float state, float relative) {
	if(isCurrentState(PS_IDLE)) {
		if (a == InputDevice::AI_LX)
			setState(PS_WALKING);
	}
}

void Scavenger::Player::onDisconnected(Dojo::InputDevice* j) {

}