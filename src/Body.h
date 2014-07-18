#ifndef Header_h__
#define Header_h__

#include "common_header.h"

namespace Scavenger {

	class Material;

	namespace Phys {

		enum Group {
			Background,
			Actor
		};

		class Body
		{
		public:

			Body();

			virtual float getMass() const;

			void addPolyShape(const Material& material, const std::vector<b2Vec2>& points, bool sensor = false);
			void addBoxShape(const Material& material, const Vector& dimensions, const Vector& center = Vector::ZERO, bool sensor = false);
			void addCircleShape(const Material& material, float radius, const Vector& center = Vector::ZERO, bool sensor = false);

			void initPhysics(Dojo::Renderable* graphics, Group group);
			void initPhysics(Dojo::Renderable* graphics, Body* partOf = nullptr, bool fixed = false);

			void addForce(const Vector& force);

			Vector getPosition() const;

			Group getGroup() const {
				return group;
			}

			b2Body* getBody() const {
				return body;
			}

			virtual void onCollision(Body* other, float force);

			void updatePhysics();

		protected:

			Dojo::Renderable* graphics;

			b2Body* body;
			Group group;

			void _addShape(b2Shape& shape, const Material& material, bool sensor);
		private:
		};
	}
}

#endif // Header_h__
