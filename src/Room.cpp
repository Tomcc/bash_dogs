#include "stdafx.h"

#include "Room.h"
#include "TilePos.h"
#include "Level.h"
#include "TerrainSource.h"
#include "Material.h"

using namespace Scavenger;

Scavenger::Room::Room(Level& level, const Vector& pos, const Vector& dimensions ) :
Object(&level, pos, dimensions),
position(pos),
level(level)
{
	logchanges = false;
}

Color::RGBAPixel& Scavenger::Room::_at(const TilePos& p)
{
	return buffer[(p.x-min.x) + (dim.y - (p.y-min.y) - 1) * dim.x];
}

bool Scavenger::Room::_isInside(const TilePos& p)
{
	return p.x >= min.x && p.x < max.x &&
		p.y >= min.y && p.y < max.y;
}

void Scavenger::Room::_floodFillCellFrom(const TilePos& start, const std::function<void(const TilePos& p, bool surface)> border) {

	std::unordered_set<TilePos> visited;

	std::deque<TilePos> queue;
	queue.emplace_back(start);

	const auto color = _at(start);
	visited.insert(start);

	while (!queue.empty()) {

		auto p = queue.front();
		queue.pop_front();

		bool isBorder = false;
		bool isSurface = false;

		TilePos positions[4] = {
			p.above(),
			p.below(),
			p.left(),
			p.right()
		};

		for (auto& n : positions) {
			bool border = !_isInside(n);
				
			int nc;
			if (!border) {
				nc = _at(n);
				border = nc != color;

				Color c(nc);
				if (border && c.a == 0)
					isSurface = true;
			}

			if (border)
				isBorder = true;
			else if (visited.count(n) == 0) {
				queue.emplace_back(n);
				visited.insert(n);
			}
		}

		if (isBorder)
			border(p, isSurface);
	}
}

bool Scavenger::Room::_isSurface(const TilePos& pos, TerrainSource& source) {
	if (!source.at(pos.above()).solid() ||
		!source.at(pos.below()).solid() ||
		!source.at(pos.left()).solid() ||
		!source.at(pos.right()).solid())
		return true;
	else
		return false;
}

void Room::generate(TerrainSource& terrainSource)
{
	max = position + getHalfSize();
	min = position - getHalfSize();

	dim = max - min;

	int spawnX = min.x + dim.x / 2;

	buffer.resize(dim.x * dim.y, 0x00ffffff);

	struct VoronoiCell {
		TilePos center;
		bool surface;
		VoronoiCell(const TilePos& center) : center(center), surface(false) {}
	};

	std::vector<VoronoiCell> voronoiCells;
	std::vector<b2Vec2> physPositions;

	bool solid = true;
	for (TilePos pos = min; pos.x < max.x; ++pos.x) {
		for (pos.y = min.y; pos.y < max.y; ++pos.y) {

			auto& t = terrainSource.at(pos);

			if (solid && !t.solid() && pos.x == spawnX) {
				solid = false;
				spawn = pos;
			}

			if (t.solid()) {
				if (Math::oneEvery(40) || 
					((!terrainSource.at(pos.above()).solid() ||
					!terrainSource.at(pos.below()).solid() ||
					!terrainSource.at(pos.left()).solid() ||
					!terrainSource.at(pos.right()).solid())) &&
					Math::oneEvery(3))
					voronoiCells.emplace_back(pos);
			}
		}
	}

	for (TilePos pos = min; pos.x < max.x; ++pos.x) {
		for (pos.y = min.y; pos.y < max.y; ++pos.y) {

			auto& t = terrainSource.at(pos);

			if (t.solid()) {
				//find closest
				float mindist = FLT_MAX;
				VoronoiCell* mincell = nullptr;
				for (auto& cell : voronoiCells) {

					float dist = pos.distanceSqr(cell.center);
					if (dist < mindist) {
						mindist = dist;
						mincell = &cell;
					}
				}

				_at(pos) = (int)mincell->center.hashCode() | 0xff000000;
			}
		}
	}

	Tessellation tess;

	//main physics
	b2Body* body;
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;

		body = level.getBox2D().CreateBody(&bodyDef);
	}

	//Tessellate all voronoi cells
	int meshID = 0;
	std::vector<bool> positionIsSurface;
	for (auto& cell : voronoiCells) {
		if (!terrainSource.at(cell.center).solid())
			continue;

		tess.positions.clear();
		positionIsSurface.clear();

		_floodFillCellFrom(cell.center, [this, &tess, &cell, &positionIsSurface](const TilePos& point, bool surface){
			const float o = TilePos::SCALE * 0.5f;
			Vector center = (Vector)point + Vector(o,o);

			float alpha = Math::rangeRandom(0, Math::PI);
			for (int i = 0; i < 3; ++i, alpha += (2.f / 3.f)*Math::PI) {
				Vector offset(
					o*cosf(alpha),
					o*sinf(alpha));

				if (surface)
					cell.surface = true;
				
				tess.positions.emplace_back(center + offset);
				positionIsSurface.push_back(surface);
			}
		});

		Tessellation::Position bbmax(-DBL_MAX, -DBL_MAX), bbmin(DBL_MAX, DBL_MAX);
		for (auto& p : tess.positions) {
			bbmax.x = std::max(p.x, bbmax.x);
			bbmax.y = std::max(p.y, bbmax.y);

			bbmin.x = std::min(p.x, bbmin.x);
			bbmin.y = std::min(p.y, bbmin.y);
		}

		if (tess.positions.size() < 3 || bbmax.x - bbmin.x < TilePos::SCALE || bbmax.y - bbmin.y < TilePos::SCALE)
			continue;

		//generate a CCW hull around the points
		tess.tessellate( Tessellation::GENERATE_HULL, 10000);

		if (tess.outIndices.empty())
			continue;

		auto mesh = new Mesh(this);
		addMesh(mesh, "rock" + String(meshID++));

		mesh->setVertexFieldEnabled(Mesh::VF_POSITION2D);
		mesh->setVertexFieldEnabled(Mesh::VF_COLOR);
		mesh->setTriangleMode(Mesh::TM_TRIANGLE_LIST);

		mesh->begin(tess.outIndices.size());

		int idx = 0;
		for (int i = 0; i < tess.positions.size(); ++i) {
			auto& pos = tess.positions[i];
			mesh->vertex(Vector(pos.x, pos.y));

			Color c =(((idx++) % 3) != 0) ? 0xff2d282d : 0xff332d31;
			if (!positionIsSurface[i])
				c *= 0.8f;
			else {
				c.r *= 1.1f;
			}

			if (!cell.surface)
				c *= 0.9f;

			mesh->color(c);
		}

		for (auto& i : tess.outIndices)
			mesh->index(i);

		mesh->end();
 
 		Renderable* test = new Renderable(&level, Vector::ZERO, mesh);
		level.addChild(test, Layers::LL_ACTORS);

		//Physics
		//if (cell.surface)
		{
			physPositions.clear();

			//compose the vertices (already CCW) in the phys mesh
			auto& p = tess.positions[tess.outHullSegments[0].i2];
			physPositions.emplace_back(p.x, p.y);
			for (auto& segment : tess.outHullSegments) {
				auto& p = tess.positions[segment.i1];
				physPositions.emplace_back(p.x, p.y);
			}

			DEBUG_ASSERT(physPositions.size() < b2_maxPolygonVertices, "Created a too complex polygon");

			b2PolygonShape shape;
			shape.Set(physPositions.data(), physPositions.size());

			b2FixtureDef fix;
			fix.shape = &shape;

			body->CreateFixture(&fix);
		}
	}


	//HACK make the background brown again
	for (TilePos pos = min; pos.x < max.x; ++pos.x) {
		for (pos.y = min.y; pos.y < max.y; ++pos.y) {
			auto& col = _at(pos);

			if (col & 0xff000000) //has alpha
				col = _isSurface(pos, terrainSource) ? 0 : 0xff181820;
		}
	}



	Texture* t = new Texture();
	t->disableTiling();
	t->disableMipmaps();
	t->disableBilinearFiltering();
	t->loadFromMemory((byte*)buffer.data(), dim.x, dim.y, GL_RGBA, GL_RGBA);

	Renderable* quad = new Renderable(&level, Vector::ZERO, "texturedQuad");
	quad->scale = getSize();
	quad->setTexture(t);
	addChild(quad, Layers::LL_TERRAIN);
}



