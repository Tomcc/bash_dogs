#pragma once

#include "common_header.h"

namespace Scavenger {
	class TilePos;
	class Material;

	class TerrainSource
	{
	public:

		virtual Material& at(const TilePos& pos) const = 0;

		virtual TilePos findSurfacePosAt(int x) const;

	protected:
	private:
	};
}

