#pragma once

#include "common_header.h"

namespace bash_dogs {
	class File;
	class Level;

	class FileSystem : public Object
	{
	public:

		const bool graphics;

		FileSystem(Level & level, const Vector& pos, bool graphics = true);

		void initialize();

		virtual void onAction(float dt);

	protected:

		File* root;

		void _makeSubFile(File& parent, int step, int& cachesPlaced, std::vector<File*>& nodes);
	private:
	};
}



