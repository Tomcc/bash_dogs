#include "stdafx.h"

#include "DynamicLight.h"
#include "Level.h"
#include "Body.h"

using namespace Scavenger;
using namespace Phys;

DynamicLight::DynamicLight(GameState& level, const Vector& offset, Body* except /*= nullptr*/) :
Renderable(&level, offset, new Mesh(&level)),
caster(except) {

	mesh->setVertexFieldEnabled(Mesh::VF_POSITION2D);
	mesh->setVertexFieldEnabled(Mesh::VF_COLOR);
	mesh->setTriangleMode(Mesh::TM_LINE_LIST);
	mesh->setDynamic(true);
}

Scavenger::DynamicLight::ShadowRay::ShadowRay(const Vector& hit, const Vector& normal, const Vector& start) :
hit(hit),
normal(normal),
length(hit.distance(start)) {
	Vector diff = hit - start;

	angle = atan2f(diff.y, diff.x);
}

DynamicLight::Layer& DynamicLight::addLayer(const Color& color, float radius, const Vector& coneDir /* = Vector::UNIT_X */, float coneWidth /* = 2.f */, bool debug /* = false */) {
	layers.emplace_back(new Layer(*this, color, radius, coneDir, coneWidth, debug));
	getGameState()->addChild(layers.back(), Layers::LL_LIGHT);

// 	if (!debug)
// 		addLayer(color, radius, coneDir, coneWidth, true);

	return *layers.back();
}

bool Scavenger::DynamicLight::_ray(const Vector& start, Vector target, const Vector& normal1 /*= Vector::ZERO*/, const Vector& normal2 /*= Vector::ZERO*/) {

	Vector end, dir = (target - start).normalized();

	float d1 = dir*normal1;
	float d2 = dir*normal2;

	const float r = 15.f;
	if (d1 * d2 < 0) { //divergent signs, it's a vertex on the boundary between visible and not visible - cast TWO rays

		//get exiting vector
		Vector off = (normal1 + normal2) * 0.5f;
		off = off.normalized() * 0.01f;

		end = start + Vector(target - off - start).normalized() * r;
		
		auto res = ((Level*)getGameState())->raycast(start, end, caster);
		shadowRayQueue.emplace_back(res.position, res.hit ? res.normal : -dir, start);

		end = start + Vector(target + off - start).normalized() * r;

		res = ((Level*)getGameState())->raycast(start, end, caster);
		shadowRayQueue.emplace_back(res.position, res.hit ? res.normal : -dir, start);
		return true;


	} else 
		if (d1 < 0 || d2 < 0 || (d1 == 0 && d2 == 0)) {

		end = start + dir * r;

		auto res = ((Level*)getGameState())->raycast(start, end, caster);
		shadowRayQueue.emplace_back(res.position, res.hit ? res.normal : -dir, start);
		return true;
	}
	else return false;
}

void DynamicLight::onAction(float dt) {
	
	Object::onAction(dt);

	auto& box2D = ((Level*)getGameState())->getBox2D();
	auto& bound = *getGameState()->getViewport();

	//get a list of fixtures in the view
	class Query : public b2QueryCallback
	{
	protected:
		b2AABB screen;
	public:

		Query(const Object& bounds) {
			screen.lowerBound = b2Vec2(bounds.getWorldMin().x, bounds.getWorldMin().y);
			screen.upperBound = b2Vec2(bounds.getWorldMax().x, bounds.getWorldMax().y);
		}

		void run(b2World& box2D) {
			box2D.QueryAABB(this, screen);
		}

		virtual bool ReportFixture(b2Fixture* fixture) {
			fixtures.push_back(fixture);
			return true;
		}

		std::vector<b2Fixture*> fixtures;
	};

	//list all the shapes in the screen
	Query query( bound );
	query.run(box2D);

	mesh->begin();

	Vector center = caster ? caster->getPosition() : position;
	for (auto fixture : query.fixtures) {
		if (fixture->GetShape()->GetType() != b2Shape::e_polygon || fixture->GetUserData() == caster )
			continue;

		b2PolygonShape& shape = *(b2PolygonShape*)fixture->GetShape();
		int l = shape.m_count - 1;
		Vector lastNormal(shape.m_normals[l].x, shape.m_normals[l].y);
		for (int i = 0; i < shape.m_count; ++i) {
			Vector p(shape.m_vertices[i].x, shape.m_vertices[i].y);

			Vector normal = Vector(shape.m_normals[i].x, shape.m_normals[i].y);
			bool visible = _ray(center, p, normal, lastNormal);

			//show normals
			{
				mesh->vertex(p);
				mesh->color(visible ? Color::RED : Color::BLUE);
				mesh->vertex(p + normal * 0.1f);
				mesh->color(visible ? Color::RED : Color::BLUE);
			}

			lastNormal = normal;
		}
	}

	mesh->end();

	//cast 4 rays in the 4 directions to avoid degenerate triangles
	float r = 10.f;

	for (auto& layer : layers) {
	
		if (layer->isLimited()) {
			float alpha = Math::PI * layer->coneWidth;

			Quaternion icantdotrig(alpha, Vector::UNIT_Z);

			Vector A = (icantdotrig * layer->coneDir) * r;
			Vector B = ((-icantdotrig) * layer->coneDir) * r;

			_ray(center, center + A);
			_ray(center, center + (layer->coneDir * r));
			_ray(center, center + B);
		}
		else {
			auto min = bound.getWorldMin();
			auto max = bound.getWorldMax();

			_ray(center, max);
			_ray(center, min);
			_ray(center, Vector(max.x, min.y));
			_ray(center, Vector(min.x, max.y));
		}
	}

	std::sort(shadowRayQueue.begin(), shadowRayQueue.end(), [](const ShadowRay& a, const ShadowRay& b) {
		return a.angle < b.angle;
	});

	//update the light meshes
	for (auto& layer : layers)
		layer->build(shadowRayQueue, center);

	shadowRayQueue.clear();
}

void DynamicLight::Layer::_uv(const Vector& top, const Vector& start) {
	Vector halfSize = radius;
	Vector min = start - halfSize;
	Vector dim = halfSize*2.f;

	Vector uv = (top - min) / dim;
	mesh->uv(uv.x, uv.y);
}

void Scavenger::DynamicLight::Layer::build(const ShadowRayList& rays, const Vector& center) {
	mesh->begin();

	if (!debug) {
		mesh->vertex(center);
		mesh->color(color);
		_uv(center, center);

		int lastIDX = rays.size() - 1;
		for (int i = 0; i < rays.size(); ++i) {

			auto& ray = rays[i];
			auto& last = rays[lastIDX];

			//if (last.normal != top.normal) 
			float angle = 0;
			if (coneDir != Vector::ZERO)
				angle = (1.f + (coneDir * (ray.hit - center).normalized())) * 0.5f;

			//if (angle < coneWidth)

			//	if ((last.normal * ray.normal) < 0.95f || std::abs(last.length - ray.length) > 0.02f || i == 0)
			{
				auto& p1 = ray.hit;
				auto& p2 = last.hit;

				int base = mesh->getVertexCount();

				mesh->vertex(p1); //0
				mesh->color(color);
				_uv(p1, center);

				mesh->vertex(p2); //1
				mesh->color(color);
				_uv(p2, center);

				mesh->triangle(base, 0, base + 1);

// 				//extend
				float ext = 0.15f;
				Vector extA = (p1 - center).normalized() * ext + p1;
				Vector extB = (p2 - center).normalized() * ext + p2;

				mesh->vertex(extA); //2
				mesh->color(Color::BLACK);
				_uv(extA, center);

				mesh->vertex(extB); //3
				mesh->color(Color::BLACK);
				_uv(extB, center);

				mesh->triangle(base, base + 3, base + 2);
				mesh->triangle(base, base + 1, base + 3);

				lastIDX = i;
			}
		}
	}
	else {
		for (int i = 0; i < rays.size(); ++i) {
			auto& ray = rays[i];

			mesh->vertex(center);
			mesh->color(Color::WHITE);

			mesh->vertex(ray.hit); //1
			mesh->color(Color::WHITE);

		}
	}
	mesh->end();
}

DynamicLight::Layer::Layer(Object& parent, const Color& color_, float radius, const Vector& coneDir, float coneWidth, bool debug) :
Renderable(&parent, Vector::ZERO, new Mesh(parent.getGameState())),
radius(radius),
coneWidth(coneWidth),
coneDir(coneDir),
debug(debug) {
	color = color_;

	mesh->setVertexFieldEnabled(Mesh::VF_POSITION2D);
	mesh->setVertexFieldEnabled(Mesh::VF_COLOR);
	
	if (!debug) {
		mesh->setVertexFieldEnabled(Mesh::VF_UV_0);
		mesh->setTriangleMode(Mesh::TM_TRIANGLE_LIST);

		setTexture(parent.getGameState()->getTexture("torch"));
		getTexture()->disableTiling();
		
		setBlending(RenderState::BM_ADD);
	}
	else {
		mesh->setTriangleMode(Mesh::TM_LINE_LIST);
	}

	mesh->setDynamic(true);

	cullMode = RenderState::CM_DISABLED;
}
