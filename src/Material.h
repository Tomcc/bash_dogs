#pragma once

#include "common_header.h"

namespace Scavenger {

	class Material
	{
	public:

		enum State {
			Gas,
			Fluid,
			Solid
		};

		const int id;

		State state = Solid;

		float density = 1.f;

		//fluid parameters
		float viscosity = 1.f; 

		//solid parameters
		float friction = 0.5f;
		float restitution = 0.f;

		Material();

		Material(int id);

		bool solid() const {
			return state == Solid;
		}

	protected:

	private:
	};

}

