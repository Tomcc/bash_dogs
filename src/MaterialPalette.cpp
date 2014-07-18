#include "stdafx.h"

#include "MaterialPalette.h"
#include "Material.h"

using namespace Scavenger;

Material& Scavenger::TerrainMaterialPalette::_makeMaterial(const String& name) {
	auto id = assignedIDs++;
	return *(nameMap[name] = (materials[id] = make_unique<Material>(id)).get());
}

TerrainMaterialPalette::TerrainMaterialPalette(Dojo::Table& desc) {

	//TODO load from file because yes

	auto& air = _makeMaterial("air");
	air.state = Material::State::Gas;

	_makeMaterial("stone");
}

Material* Scavenger::TerrainMaterialPalette::getNamed(const String& name) {
	auto elem = nameMap.find(name);
	return elem == nameMap.end() ? nullptr : elem->second;
}

Material& Scavenger::TerrainMaterialPalette::getID(TerraiMaterialID id) {

	DEBUG_ASSERT(id >= 0 && id < MAX_TERRAIN_MATERIAL_ID, "Invalid material ID");

	auto& material = materials[id];
	DEBUG_ASSERT(material, "This material does not exist");

	return *material;
}
