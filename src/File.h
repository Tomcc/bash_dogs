#pragma once

#include "common_header.h"

class b2Body;

namespace bash_dogs {
	class FileSystem;

	class File : public Object
	{
	public:

		//HACK
		b2Body* body = nullptr;

		enum State {
			FS_UNKNOWN,
			FS_LOCKED,
			FS_OPEN,
			FS_DELETED
		};

		enum Type {
			T_FILE,
			T_FOLDER,
			T_CACHE
		};

		const Type type;

		File(FileSystem& parent, const Vector&pos, const String& name, Type type, State initialState);

		File(FileSystem& parent,Unique<Table> desc);

		File& addFile(File& file);

		virtual void onAction(float dt);

		Unique<Table> serialize() const;

		const std::vector<File*>& getSubFiles() const {
			return subFiles;
		}

	protected:
		TextArea* label;
		Renderable* icon;

		FileSystem& fs;

		const String name;
		State state;

		float timer = 0;

		Vector localPos;

		std::vector<File*> subFiles;

		static Mesh* _meshForState(ResourceGroup& rg, State state, Type type);
	private:
	};
}



