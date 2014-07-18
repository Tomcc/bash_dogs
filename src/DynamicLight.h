#pragma once

#include "common_header.h"

namespace Scavenger {

	namespace Phys {
		class Body;
	}

	class DynamicLight : public Renderable
	{
	protected:

		struct ShadowRay {
			Vector hit, normal;
			float angle, length;
			ShadowRay(const Vector& hit, const Vector& normal, const Vector& start);
		};
		typedef std::vector<ShadowRay> ShadowRayList;

	public:

		struct Layer : public Renderable
		{
			float radius;
			Vector coneDir;
			float coneWidth;
			bool debug;

			Layer(Object& parent, const Color& color, float radius, const Vector& coneDir, float coneWidth, bool debug);

			bool isLimited() const {
				return coneDir != Vector::ZERO;
			}

			void build(const ShadowRayList& rays, const Vector& center);

		protected:
			void _uv(const Vector& top, const Vector& start);
		};

		DynamicLight(GameState& level, const Vector& offset, Phys::Body* caster = nullptr);

		Layer& addLayer(const Color& color, float radius, const Vector& coneDir = Vector::ZERO, float coneWidth = 2.f, bool debug = false);

		virtual void onAction(float dt) override;
	
	protected:
		Phys::Body* caster;

		std::vector<Layer*> layers;
		ShadowRayList shadowRayQueue;

		bool _ray(const Vector& start, Vector target, const Vector& normal1 = Vector::ZERO, const Vector& normal2 = Vector::ZERO);
	};

}



