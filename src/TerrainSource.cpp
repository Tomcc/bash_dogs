#include "stdafx.h"

#include "TerrainSource.h"
#include "TilePos.h"
#include "Material.h"

using namespace Scavenger;


Scavenger::TilePos Scavenger::TerrainSource::findSurfacePosAt(int x) const
{
	int bounce = 1000;

	TilePos surface(x, 0);

	bool wasSolid = at(surface).solid();

	while (bounce > 1) {

		surface.y += bounce * (wasSolid ? 1 : -1);

		bool solid = at(surface).solid();
		if (solid != wasSolid) {
			wasSolid = solid;

			bounce /= 2;
		}
	}
	return surface; //good enough?
}


