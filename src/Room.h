#pragma once

#include "common_header.h"
#include "TilePos.h"

namespace Scavenger {

	class Level;
	class TerrainSource;

	class Room : 
		public Object,
		public ResourceGroup
	{
	public:
		Vector spawn;

		Room(Level& level, const Vector& pos, const Vector& dimensions);

		virtual void generate(TerrainSource& source);

	protected:
		Level& level;
		const Vector& position;

		Renderable* graphics;

		//generation stuff
		TilePos max, min, dim;
		std::vector<Color::RGBAPixel> buffer;

		Color::RGBAPixel& _at(const TilePos& p);
		bool _isInside(const TilePos& p);

		void _floodFillCellFrom(const TilePos& p, const std::function<void(const TilePos& p, bool surface)> border);
		bool _isSurface(const TilePos& pos, TerrainSource& source);
	private:
	};
}



