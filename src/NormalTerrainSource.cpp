#include "stdafx.h"

#include "NormalTerrainSource.h"

#include "MaterialPalette.h"
#include "TilePos.h"

using namespace Scavenger;

Scavenger::NormalTerrainSource::NormalTerrainSource(TerrainMaterialPalette& palette) :
palette(palette) {

	Random r;
	mountainNoise = make_unique<Noise>(r);

}

Material& Scavenger::NormalTerrainSource::at(const TilePos& pos) const {

	const float SEA_LEVEL = 000.f;
	const float MOUNTAIN_HSCALE = 120.f;

	struct { float x, y; } scales[] = {
		{ 1123.f, 211.f },
		{ 450.f, 157.f },
		{ 1117.f, 151.f },
		{ 57.f, 157.f },
		{ 31.f, 211.f },
		{ 4.f, 25.f },
	};

	float f = SEA_LEVEL;
	for (auto& scale : scales) {
		float r = mountainNoise->filternoise(pos.x, pos.y, scale.x);
		f += (r * 0.5f) * scale.y;
	}

	return palette.getID(((float)pos.y > f) ? 0 : 1);
}
