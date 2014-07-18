#pragma once

#include "common_header.h"
#include "Body.h"
#include "DynamicLight.h"

namespace Scavenger {

	class Player :
		public Renderable,
		public InputDevice::Listener,
		public StateInterface,
		public Phys::Body
	{
	public:

		enum State {
			PS_IDLE,
			PS_WALKING,
			PS_JUMPING,
			PS_FALLING
		};

		Player(GameState& gameState, const Vector& pos);

		void assignDevice(InputDevice* device);

		virtual void onButtonPressed(Dojo::InputDevice* j, int action);
		virtual void onButtonReleased(Dojo::InputDevice* j, int action);
		virtual void onAxisMoved(Dojo::InputDevice* j, Dojo::InputDevice::Axis a, float state, float speed);
		virtual void onDisconnected(Dojo::InputDevice* j);

		virtual void onCollision(Body* other, float force) override;

		virtual void onAction(float dt);

		virtual void onDestruction();

	protected:

		virtual void onStateBegin();
		virtual void onStateLoop(float dt);
		virtual void onStateEnd();

		bool _checkGround();
		bool _move(float maxSpeed, float push);

		bool jump = false, doubleJump = false;
		int jumpBoostCountDown;
		InputDevice* device = nullptr;

		DynamicLight* torch;
	private:
	};
}