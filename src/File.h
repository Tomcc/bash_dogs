#pragma once

#include "common_header.h"

class b2Body;

namespace bash_dogs {
	class FileSystem;

	class File : public Object
	{
	public:
		const String name;

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

		bool isParentOf(File& file) const;

		virtual void onAction(float dt);

		Unique<Table> serialize() const;

		const std::vector<File*>& getSubFiles() const {
			return subFiles;
		}

		bool isDeleted() const {
			return state == FS_DELETED;
		}
		bool isLocked() const {
			return state == FS_LOCKED;
		}

		void select(bool selected);

	protected:
		TextArea* label;
		Renderable* icon;

		FileSystem& fs;

		State state;

		bool selected = false;
		float baseScale = 0.5f;

		float timer = 0;

		Vector localPos;

		std::vector<File*> subFiles;

		static Mesh* _meshForState(ResourceGroup& rg, State state, Type type);
	private:
	};
}



