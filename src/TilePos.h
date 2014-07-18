#pragma once

#include "common_header.h"

namespace Scavenger {
	class TilePos {

	public:

		static const float SCALE;

		int x;
		Height y;
		Layer z;

		TilePos() : x(0), y(0), z(0) {

		}

		TilePos(int x, Height y, Layer z = 0) :
		x(x), y(y), z(z) {

		}

		TilePos(Vector pos) {
			pos /= SCALE;
			x = (int)floorf(pos.x);
			y = (Height)floorf(pos.y);
			z = (Layer)floorf(pos.z);
		}

		TilePos& operator=(Vector pos) {
			pos /= SCALE;
			x = (int)floorf(pos.x);
			y = (Height)floorf(pos.y);
			z = (Layer)floorf(pos.z);
			return *this;
		}

		TilePos above() const {
			return TilePos(x, y + 1);
		}

		TilePos below() const  {
			return TilePos(x, y - 1);
		}

		TilePos right() const {
			return TilePos(x + 1, y);
		}

		TilePos left() const {
			return TilePos(x - 1, y);
		}

		TilePos operator-(const TilePos& rhs) const {
			return TilePos(x - rhs.x, y - rhs.y, z - rhs.z);
		}

		bool operator==(const TilePos& pos) const {
			return x == pos.x && y == pos.y && z == pos.z;
		}

		operator Vector() const {
			return Vector(x * SCALE, y * SCALE, z * SCALE);
		}
		float distanceSqr(const TilePos& cell) const {
			float a = (float)(cell.x - x);
			float b = (float)(cell.y - y);
			return a*a + b*b;
		}
		size_t hashCode() const {
			return x * 8976890 + y * 981131 + z;
		}
	};
}

namespace std
{
	using namespace Scavenger;
	///hash specialization for unordered_maps
	template<>
	struct hash< TilePos >
	{
		// hash functor for vector
		size_t operator()(const TilePos& _Keyval) const
		{
			return _Keyval.hashCode();
		}
	};
}