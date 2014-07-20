#pragma once

#include "common_header.h"

namespace bash_dogs {
	class File;
	class Level;

	class FileSystem : public Object	{
	public:

		const bool graphics;

		FileSystem(Level & level, const Vector& pos, bool graphics = true);

		void initialize();

		virtual void onAction(float dt);

		void select(const String& file);

		void _fileAdded(File& file);

		File* getFile(const String& name) const;

		File& getSelected() const;

		bool canMoveBetween(File& A, File& B) const;

	protected:

		File* root, *selected = nullptr;
		std::unordered_map<String, File*> nameMap;

		void _makeSubFile(File& parent, int step, int& cachesPlaced, std::vector<File*>& nodes);
	private:
	};
}



