#pragma once

#include "common_header.h"

namespace Scavenger {

	class Material;

	class TerrainMaterialPalette
	{
	public:

		TerrainMaterialPalette(Dojo::Table& desc);

		Material* getNamed(const String& name);

		Material& getID(TerraiMaterialID id);


	protected:

		int assignedIDs = 0;
		std::unordered_map< String, Material* > nameMap;

		Unique<Material> materials[MAX_TERRAIN_MATERIAL_ID];

		Material& _makeMaterial(const String& name);

	private:
	};
}

