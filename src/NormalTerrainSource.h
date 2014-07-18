#pragma once

#include "common_header.h"

#include "TerrainSource.h"

namespace Scavenger {

	class TerrainMaterialPalette;

	class NormalTerrainSource : 
		public TerrainSource
	{
	public:

		NormalTerrainSource( TerrainMaterialPalette& palette );

		virtual Material& at(const TilePos& pos) const;

	protected:

		TerrainMaterialPalette& palette;

		Unique<Noise> mountainNoise;
	};
}

