#pragma once

#include "common_header.h"

namespace bash_dogs {
	class File;
	class Level;

	class FileSystem : public Object
	{
	public:

		FileSystem(Level & level, const Vector& pos);

		virtual void onAction(float dt);

	protected:

		File* root;
	private:
	};
}



